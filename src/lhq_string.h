/*
 * Copyright 2018 Solus Project <copyright@getsol.us>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http: *www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LINUX_HARDWARE_QUALIFIER_STRING_H__
#define __LINUX_HARDWARE_QUALIFIER_STRING_H__

#include <stdlib.h>

/* Wrapper for strings (safety) */
typedef struct {
    char    *contents;
    size_t  length;
} LHQ_STRING;

/* Create a new LHQ_STRING

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @retruens pointer to ther new LHQ_STRING
*/
LHQ_STRING* lhq_string_new(char * contents, size_t length) {
    LHQ_STRING *result = (LHQ_STRING*)calloc(1,sizeof(LHQ_STRING));
    result->contents = contents;
    result->length = length;
    return result;
}

/* Destroy an LHQ_STRING */
void lhq_string_free(LHQ_STRING *string) {
    if(string != NULL) {
        free(string->contents);
    }
    free(string);
}

#endif
