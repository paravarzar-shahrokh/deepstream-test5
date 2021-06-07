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
#include <stddef.h>
#include <string.h>
#include "mjson.h"

#define SIZE 64
#define MAXOBJECTS 8

/* Data object to model */
struct my_object {
    char text[SIZE];
    bool flag;
    int count;
};

/* List of objects */
struct my_object_list {
    int nobjects;
    struct my_object list[MAXOBJECTS];
};

/* Object specific parsing function */
int json_object_list_read(const char *buf, struct my_object_list *obj_list) {

    /* Mapping of JSON attributes to my_object's struct members */
    const struct json_attr_t json_attrs_subobject[] = {
        {"text", t_string, STRUCTOBJECT(struct my_object, text),
                .len = sizeof(obj_list->list[0].text)},
        {"flag", t_boolean, STRUCTOBJECT(struct my_object, flag)},
        {"count", t_integer, STRUCTOBJECT(struct my_object, count)},
        {NULL},
    };

    /* Mapping of JSON attributes to my_object_list's struct members */
    const struct json_attr_t json_attrs_objects[] = {
        {"class", t_check, .dflt.check = "OBJECTS"},
        {"my_objects", t_array, STRUCTARRAY(obj_list->list,
                json_attrs_subobject,
                &(obj_list->nobjects))},
        {NULL},
    };

    memset(obj_list, '\0', sizeof(*obj_list));
    return json_read_object(buf, json_attrs_objects, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./json_demo_compound <input JSON string>\n");
        return -1;
    }

    /* Allocate space for object list */
    struct my_object_list *obj_list = malloc(sizeof(struct my_object_list));

    /* Call object list parsing function */
    int status = json_object_list_read(argv[1], obj_list);

    if (status == 0) {
        int i;
        for (i = 0; i < obj_list->nobjects; i++) {
            printf("Object %d:\n", i);
            printf("  text: %s\n", obj_list->list[i].text);
            printf("  flag: %s\n", obj_list->list[i].flag ? "true" : "false");
            printf("  count: %d\n", obj_list->list[i].count);
        }
    } else {
        puts(json_error_string(status));
    }

    return status;
}
