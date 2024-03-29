/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 * author: Zhihua Wang, hogan.wang@rock-chips.com
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __PLAY_WAV_H__
#define __PLAY_WAV_H__

#include <stdbool.h>

#define AUTHORIZE_FAIL_WAV "./audio/authorize_fail.wav"
#define WELCOME_WAV "./audio/welcome.wav"
#define REGISTER_SUCCESS_WAV "./audio/register_success.wav"
#define REGISTER_ALREADY_WAV "./audio/register_already.wav"
#define REGISTER_START_WAV "./audio/register_start.wav"
#define REGISTER_TIMEOUT_WAV "./audio/register_timeout.wav"
#define REGISTER_LIMIT_WAV "./audio/register_limit.wav"
#define DELETE_SUCCESS_WAV "./audio/delete_success.wav"
#define DELETE_START_WAV "./audio/delete_start.wav"
#define DELETE_TIMEOUT_WAV "./audio/delete_timeout.wav"
#define PLEASE_GO_THROUGH_WAV "./audio/please_go_through.wav"
#define RECOGNIZE_SUCC_WAV "./audio/recognition_successful.wav"


void play_wav_signal(const char *name);
void play_wav_stop();
int play_wav_thread_init(void);
void play_wav_thread_exit(void);

#endif
