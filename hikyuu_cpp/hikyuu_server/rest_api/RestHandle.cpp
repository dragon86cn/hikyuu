/*
 *  Copyright(C) 2021 hikyuu.org
 *
 *  Create on: 2021-03-07
 *     Author: fasiondog
 */

#include <unordered_map>
#include "RestHandle.h"

namespace hku {

enum hku_rest_errno {
    HKU_HANDLE_INVALID_CONTENT = 1,
    HKU_HANDLE_MISS_TOKEN = 2,
    HKU_HANDLE_UNAUTHORIZED = 3,
};

static std::unordered_map<int, const char *> g_hku_handle_errmsg{
  // clang-format off
  {HKU_HANDLE_INVALID_CONTENT, "Invalid Content-Type, Please use 'application/json'"},
  {HKU_HANDLE_MISS_TOKEN, "Missing token"},
  {HKU_HANDLE_UNAUTHORIZED, "Authentication failed"},
  // clang-format on
};

RestHandle::RestHandle(nng_aio *aio) : HttpHandle(aio) {
    addFilter([](HttpHandle *handle) {
        const char *content_type = handle->getReqHeader("Content-Type");
        HANDLE_CHECK(content_type, NNG_HTTP_STATUS_BAD_REQUEST,
                     R"({{"errcode": "{}", "msg": "{}"}})", HKU_HANDLE_INVALID_CONTENT,
                     g_hku_handle_errmsg[HKU_HANDLE_INVALID_CONTENT]);

        // const char *token = handle->getReqHeader("token");
        // HANDLE_CHECK(token, HKU_HANDLE_MISS_TOKEN);
    });
}

void RestHandle::before_run() {
    setResHeader("Content-Type", "application/json; charset=UTF-8");
}

}  // namespace hku