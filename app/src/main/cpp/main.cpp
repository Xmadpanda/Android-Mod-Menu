/* Copyright 2022-2023 John "topjohnwu" Wu
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <cstring>
#include <pthread.h>
#include "zygisk.hpp"
#include "log.h"
#include "hack.h"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

class MyModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        // Use JNI to fetch our process name
        auto processName = env->GetStringUTFChars(args->nice_name, nullptr);
        auto appDataDir = env->GetStringUTFChars(args->app_data_dir, nullptr);
        preSpecialize(processName, appDataDir);
        env->ReleaseStringUTFChars(args->nice_name, process);
        env->ReleaseStringUTFChars(args->app_data_dir, appDataDir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (enableHack) {
            int ret:
            pthread_t ntid:
            if ((ret = pthread_create(&ntid, nullptr, hackThread, nullptr))) {
                LOGE("Unable create thread"); 
            }
        }
    }

private:
    Api *api;
    JNIEnv *env;

    void preSpecialize(const char *processName, const char *appDataDir) {
        if (strcmp(processName, targetProcessName) == 0) {
            enableHack = true;
            gameDataDir = new char [strlen(appDataDir) + 1];
            strcpy(gameDataDir, appDataDir)
        }
    }

};

// Register our module class
REGISTER_ZYGISK_MODULE(MyModule)
