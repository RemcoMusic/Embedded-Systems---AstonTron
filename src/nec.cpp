#include <Arduino.h>
#include "nec.h"

NEC *NEC::singleton = 0;

#include "esp_err.h"
#ifdef TELNET_LOGGER_COMPONENT_TRUE
#include "telnet_logger.h"
#else
#include "esp_log.h"
#endif

#include "driver/periph_ctrl.h"
#include "driver/rmt.h"
#include "driver/timer.h"
#include "freertos/queue.h"

#define RMT_CHANNEL 0
#define RMT_CLK_DIV 100									 /*!< RMT counter clock divider */
#define RMT_TICK_10_US (80000000 / RMT_CLK_DIV / 100000) /*!< RMT counter value for 10 us.(Source clock is APB clock) */
#define RMT_ITEM_32_TIMEOUT_US 9500						 /*!< RMT receiver timeout value(us) */
#define RMT_RX_ACTIVE_LEVEL 0							 /*!< If we connect with a IR receiver, the data is active low */

#define NEC_HEADER_HIGH_US 9000							  /*!< NEC protocol header: positive 9ms */
#define NEC_HEADER_LOW_US 4500							  /*!< NEC protocol header: negative 4.5ms*/
#define NEC_BIT_ONE_HIGH_US 562							  /*!< NEC protocol data bit 1: positive 0.56ms */
#define NEC_BIT_ONE_LOW_US (2250 - NEC_BIT_ONE_HIGH_US)   /*!< NEC protocol data bit 1: negative 1.69ms */
#define NEC_BIT_ZERO_HIGH_US 562						  /*!< NEC protocol data bit 0: positive 0.56ms */
#define NEC_BIT_ZERO_LOW_US (1120 - NEC_BIT_ZERO_HIGH_US) /*!< NEC protocol data bit 0: negative 0.56ms */
#define NEC_BIT_END 562									  /*!< NEC protocol end: positive 0.56ms */
#define NEC_BIT_MARGIN 200								  /*!< NEC parse margin time */
#define DELAY_TIME_BETWEEN_ITEMS_MS 100

#define NEC_ITEM_DURATION(d) ((d & 0x7fff) * 10 / RMT_TICK_10_US) /*!< Parse duration time from memory register value */
#define NEC_DATA_ITEM_NUM 34									  /*!< NEC code item number: header + 32bit data + end */

#define IR_INPUT 21
#define IR_FREQUENCY 500

#define TAG "NEC-DRV"

uint16_t currentCommand = 0;
uint16_t deviceAdress = 0;

NEC *NEC::getSingleton()
{
    if (!singleton)
    {
        singleton = new NEC();
        singleton->initialize_ir();
    }
    return singleton;
}

NEC::NEC()
{
}

/**
 * @brief Initializes the IR sampling
 */
esp_err_t NEC::initialize_ir()
{

  ESP_LOGI(__FUNCTION__, "Starting IR receiver");

	ESP_LOGI(TAG, "Initializing RMT on channel %d with GPIO %d", RMT_CHANNEL, IR_INPUT);
	
	esp_err_t ret;

	rmt_config_t rmt_rx;
	rmt_rx.channel = (rmt_channel_t)RMT_CHANNEL;
	rmt_rx.gpio_num = (gpio_num_t)IR_INPUT;
	rmt_rx.clk_div = RMT_CLK_DIV;
	rmt_rx.mem_block_num = 1;
	rmt_rx.rmt_mode = RMT_MODE_RX;
	rmt_rx.rx_config.filter_en = true;
	rmt_rx.rx_config.filter_ticks_thresh = 100;
	rmt_rx.rx_config.idle_threshold = RMT_ITEM_32_TIMEOUT_US / 10 * (RMT_TICK_10_US);

	rmt_config(&rmt_rx);
	rmt_driver_install(rmt_rx.channel, 1000, 0);

	ret = ESP_OK;

	rb = NULL;

	return ret;
}

/*
 * @brief Check whether duration is around target_us
 */
inline bool nec_check_in_range(int duration_ticks, int target_us, int margin_us)
{
	if ((NEC_ITEM_DURATION(duration_ticks) < (target_us + margin_us)) && (NEC_ITEM_DURATION(duration_ticks) > (target_us - margin_us)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * @brief Check whether this value represents an NEC header
 */
static bool nec_header_if(rmt_item32_t *item)
{
	if ((item->level0 == RMT_RX_ACTIVE_LEVEL && item->level1 != RMT_RX_ACTIVE_LEVEL) && nec_check_in_range(item->duration0, NEC_HEADER_HIGH_US, NEC_BIT_MARGIN) && nec_check_in_range(item->duration1, NEC_HEADER_LOW_US, NEC_BIT_MARGIN))
	{
		return true;
	}
	return false;
}

/*
 * @brief Check whether this value represents an NEC data bit 1
 */
static bool nec_bit_one_if(rmt_item32_t *item)
{
	if ((item->level0 == RMT_RX_ACTIVE_LEVEL && item->level1 != RMT_RX_ACTIVE_LEVEL) && nec_check_in_range(item->duration0, NEC_BIT_ONE_HIGH_US, NEC_BIT_MARGIN) && nec_check_in_range(item->duration1, NEC_BIT_ONE_LOW_US, NEC_BIT_MARGIN))
	{
		return true;
	}
	return false;
}

/*
 * @brief Check whether this value represents an NEC data bit 0
 */
static bool nec_bit_zero_if(rmt_item32_t *item)
{
	if ((item->level0 == RMT_RX_ACTIVE_LEVEL && item->level1 != RMT_RX_ACTIVE_LEVEL) && nec_check_in_range(item->duration0, NEC_BIT_ZERO_HIGH_US, NEC_BIT_MARGIN) && nec_check_in_range(item->duration1, NEC_BIT_ZERO_LOW_US, NEC_BIT_MARGIN))
	{
		return true;
	}
	return false;
}

/*
 * @brief Parse NEC 32 bit waveform to address and command.
 */
static int nec_parse_items(rmt_item32_t *item, int item_num, uint16_t *addr, uint16_t *data)
{
	int w_len = item_num;
	if (w_len < NEC_DATA_ITEM_NUM)
	{
		return -1;
	}
	int i = 0, j = 0;
	if (!nec_header_if(item++))
	{
		return -1;
	}
	uint16_t addr_t = 0;
	for (j = 0; j < 16; j++)
	{
		if (nec_bit_one_if(item))
		{
			addr_t |= (1 << j);
		}
		else if (nec_bit_zero_if(item))
		{
			addr_t |= (0 << j);
		}
		else
		{
			return -1;
		}
		item++;
		i++;
	}
	uint16_t data_t = 0;
	for (j = 0; j < 16; j++)
	{
		if (nec_bit_one_if(item))
		{
			data_t |= (1 << j);
		}
		else if (nec_bit_zero_if(item))
		{
			data_t |= (0 << j);
		}
		else
		{
			return -1;
		}
		item++;
		i++;
	}
	*addr = addr_t;
	*data = data_t;
	return i;
}

void NEC::loop(int task_idx){

	rmt_channel_t channel = (rmt_channel_t)RMT_CHANNEL;
		
	size_t rx_size = 0;
	rmt_item32_t *item = NULL;
  
	if(rb){			
		//try to receive data from ringbuffer.
		//RMT driver will push all the data it receives to its ringbuffer.
		//We just need to parse the value and return the spaces of ringbuffer.
		item = (rmt_item32_t *)xRingbufferReceive(rb, &rx_size, (DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
		if(item){
			int offset = 0;

			uint16_t rmt_addr;
			uint16_t rmt_cmd;
			int res = 0;
			do 
			{	
				//parse data value from ringbuffer.
				res = nec_parse_items(item + offset, rx_size / 4 - offset, &rmt_addr, &rmt_cmd);
				if(res > 0){
					offset += res + 1;
					currentCommand = rmt_cmd;
					deviceAdress = rmt_addr;
				}else{
					break;
				}					
			}while(res != -1);
			//after parsing the data, return spaces to ringbuffer.
			vRingbufferReturnItem(rb, (void *)item);
		}
	}else{
		ESP_LOGI(TAG, "Starting ringbuffer");
		//get RMT RX ringbuffer
		rmt_get_ringbuf_handle(channel, &rb);
		rmt_rx_start(channel, 1);
	}		
}

void NEC::setCommandReceived(int x) 
{
    currentCommand = x;
}

int NEC::getCommand() 
{
	if(deviceAdress == 0xff00)
	{
		return currentCommand;
	}else
	{
		return 0;
	}
}
