# Parsing JSON in C using microjson
Alexander Cheung
## Preface
JavaScript Object Notation, or JSON, is a commonly used human-readable format for transmitting data objects as key-value pairs. Developed originally for server-browser communication, the use of JSON has since expanded into a universal data interchange format. This tutorial will provide a simple introduction to parsing JSON strings in the C programming language using the [microjson](http://www.catb.org/esr/microjson/) library. More sophisticated examples can be found in the official documentation. However, this tutorial should be enough to get you started parsing simple objects right away.

As is typical with string manipulation in C, parsing JSON is a non-trivial task best left to pre-existing libraries. The microjson library was designed to parse a large subset of JSON into static C structures using only fixed-extent storage. The library is small and has a low footprint - perfect for use in embedded systems.
## Example Compilation
To compile the example programs in a Linux or Linux-like environment, simply use the included `Makefile`:
```
> make all
```
You can also import the source files into an Eclipse or Xcode project and have the IDE compile them for you if you're afraid of the command line.
## Parsing Simple Objects
In the context of this tutorial, *simple objects* will refer to JSON objects which are not nested. For this tutorial, we will assume that the form of the JSON object is known.

This section will demonstrate how to use microjson to parse a JSON object with the following format:
```json
{"text": "sample message", "flag": true, "count": 5}
```
This object contains a string `text`, boolean `flag`, and integer `count`. This example will parse the object into a static C structure of the form:
```c
struct my_object {
    char text[SIZE];
    bool flag;
    int count;
};
```
In order to parse the JSON object into the C structure above, we need to write a parsing function that will map the fields of the JSON object to the members of the C structure:
```c
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
```
The function `json_obj_read` uses the C structure `json_attr_t` to map the corresponding fields of the JSON object to the `object` C structure. `t_<type>` and `.aadr.<type>` together are used by the library to identify the type of data to expect in the JSON object and the corresponding type of the structure member. Notice in the case of the string `text`, we must also specify the length `.len` of the character buffer.

We will use the following program to test our code:
```c
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
```
This program takes a single argument, which is the JSON string representation of the object we want to parse. The program will print each of the members of our C `object` structure after parsing the input JSON into it:
```
> ./json_demo_simple '{"text": "sample message", "flag": true, "count": 5}'
text: sample message
flag: true
count: 5
```
## Parsing Compound Objects
One of the features of JSON is the ability to nest objects within other objects. One common example is having an array of JSON objects as a value within another JSON object, such as in the following example:
```json
{"my_objects": [{"text": "sample zero", "flag": true, "count": 2}, {"text": "sample one", "flag": false, "count": 5}]}
```
Here we have two instances of our previous `my_object` within the array called `my_objects`. To handle this, we have to specify that the incoming JSON has an array and that each of the elements in the array have the form of `my_object`. We'll want to keep this array of `my_object` within another container structure as follows:
```c
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
```
Then we write the object specific parsing function for the container structure using two definitions of `json_attr_t`; one to describe the container object, and one to describe each of the objects in the array:
```c
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
```
We can test this function using the following program:
```c
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
```
This program takes a single argument, which is the JSON string representation of the list of objects we want to parse. The program will print the members of each `my_object` structure parsed from the input:
```
> ./json_demo_compound '{"my_objects": [{"text": "sample zero", "flag": true, "count": 2}, {"text": "sample one", "flag": false, "count": 5}]}'
Object 0:
  text: sample zero
  flag: true
  count: 2
Object 1:
  text: sample one
  flag: false
  count: 5
```
## References
[1] microjson official resource page: [http://www.catb.org/esr/microjson/](http://www.catb.org/esr/microjson/)
