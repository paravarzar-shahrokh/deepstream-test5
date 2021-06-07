/*
 * Copyright (C) 2015 Alexander Cheung
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include "mjson.h"

#define SIZE 64

/* Data object to model */
struct my_object {
    char text[SIZE];
    bool flag;
    int count;
};

/* Object specific parsing function */
int json_myobj_read(const char *buf, struct my_object *myobj) {

    /* Mapping of JSON attributes to C my_object's struct members */
    const struct json_attr_t json_attrs[] = {
        {"text", t_string, .addr.string = myobj->text,
                .len = sizeof(myobj->text)},
        {"flag", t_boolean, .addr.boolean = &(myobj->flag)},
        {"count", t_integer, .addr.integer = &(myobj->count)},
        {NULL},
    };

    /* Parse the JSON string from buffer */
    return json_read_object(buf, json_attrs, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./json_demo_simple <input JSON string>\n");
        return -1;
    }

    /* Allocate space for my_object */
    struct my_object *myobj = malloc(sizeof(struct my_object));

    /* Call my_object parsing function */
    int status = json_myobj_read(argv[1], myobj);

    if (status == 0) {
        printf("text: %s\n", myobj->text);
        printf("flag: %s\n", myobj->flag ? "true" : "false");
        printf("count: %d\n", myobj->count);
    } else {
        puts(json_error_string(status));
    }

    return status;
}
