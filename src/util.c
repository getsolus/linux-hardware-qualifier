/*
 * Copyright 2018-2019 Solus Project <copyright@getsol.us>
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

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int lhq_file_to_string(char *path, char *prefix, char **dest, int offset, size_t size) {
    static char buff[128];
    strcpy(buff, path);
    strcat(buff, prefix);
    FILE *f = fopen(buff, "r");
    if(f == NULL) return -1;
    if(offset != 0) {
        fseek(f, offset, SEEK_SET);
    }
    *dest = (char *)calloc(1, size + 1);
    if((fread(*dest, 1, size, f) != size) || (strnlen(*dest, size) != size)) {
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}
