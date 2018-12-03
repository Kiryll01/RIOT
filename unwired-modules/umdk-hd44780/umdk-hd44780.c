/*
 * Copyright (C) 2016-2018 Unwired Devices LLC <info@unwds.com>

 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * @defgroup    
 * @ingroup     
 * @brief       
 * @{
 * @file		umdk-hd44780.c
 * @brief       umdk-hd44780 module implementation
 * @author      Oleg Artamonov
 */

#ifdef __cplusplus
extern "C" {
#endif

/* define is autogenerated, do not change */
#undef _UMDK_MID_
#define _UMDK_MID_ UNWDS_HD44780_MODULE_ID

/* define is autogenerated, do not change */
#undef _UMDK_NAME_
#define _UMDK_NAME_ "hd44780"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"

#include "umdk-hd44780.h"

#include "umdk-ids.h"
#include "unwds-common.h"

#include "thread.h"
#include "rtctimers.h"

static hd44780_t dev;
static uwnds_cb_t *callback;

int umdk_hd44780_shell_cmd(int argc, char **argv) {
    if (argc == 1) {
        puts (_UMDK_NAME_ " print <N> <string> - print string on row N");
        puts (_UMDK_NAME_ " clear - clear display");
        return 0;
    }
    
    char *cmd = argv[1];
	
    if (strcmp(cmd, "print") == 0) {
        uint8_t col = atoi(argv[2]);
        uint8_t row = atoi(argv[3]); 
        
        hd44780_set_cursor(&dev, col, row);
        
        char line[UMDK_HD44780_COLS + 1] = { 0 };
        if (strlen(argv[4]) > UMDK_HD44780_COLS) {
            memcpy(line, argv[4], UMDK_HD44780_COLS);
        } else {
            memcpy(line, argv[4], strlen(argv[4]));
        }
        
        hd44780_print(&dev, line);
    }
    
    if (strcmp(cmd, "clear") == 0) {
        hd44780_clear(&dev);
    }
    
    return 1;
}

void umdk_hd44780_init(uwnds_cb_t *event_callback) {
	callback = event_callback;

    unwds_add_shell_command(_UMDK_NAME_, "type '" _UMDK_NAME_ "' for commands list", umdk_hd44780_shell_cmd);
    
    hd44780_init(&dev, &umdk_hd44780_params[0]);
    hd44780_clear(&dev);
    hd44780_set_cursor(&dev, 0, 0);
    hd44780_print(&dev, _UMDK_NAME_);
    hd44780_set_cursor(&dev, 0, 1);
    hd44780_print(&dev, "www.unwds.com");
}

static void reply_fail(module_data_t *reply) {
	reply->length = 2;
	reply->data[0] = _UMDK_MID_;
	reply->data[1] = 255;
}

static void reply_ok(module_data_t *reply) {
	reply->length = 2;
	reply->data[0] = _UMDK_MID_;
	reply->data[1] = 0;
}

bool umdk_hd44780_cmd(module_data_t *data, module_data_t *reply) {
	if (data->length < 1) {
		reply_fail(reply);
		return true;
	}

	umdk_hd44780_cmd_t c = data->data[0];
	switch (c) {
	case UMDK_HD44780_CMD_PRINT: {
        if (data->length < 3) {
            reply_fail(reply);
        } else {
            if (data->data[1] & 0x80) {
                hd44780_clear(&dev);
            }
            
            uint8_t row = (data->data[1] & 0x60) >> 5;         
            uint8_t col = (data->data[1] & 0x1F);
            
            hd44780_set_cursor(&dev, row, col);
            char line[UMDK_HD44780_COLS + 1] = { 0 };
            /* fill the string with spaces to clear whole line on LCD */
            memset(line, ' ', UMDK_HD44780_COLS);
            
            if (data->length - 2 > UMDK_HD44780_COLS) {
                memcpy(line, &data->data[2], UMDK_HD44780_COLS);
            } else {
                memcpy(line, &data->data[2], data->length - 2);
            }
            
            hd44780_set_cursor(&dev, col, row);
            hd44780_print(&dev, line);
            
            reply_ok(reply);
        }
		break;
	}
	default:
		reply_fail(reply);
		break;
	}

	return true;
}

#ifdef __cplusplus
}
#endif
