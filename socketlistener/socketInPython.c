#include <stdio.h>
// #include <conio.h>
// #include<ncurses.h>
#include </usr/include/python2.7/Python.h>


int main(int argc, char *argv[])
{

    Py_Initialize();
	// Get a reference to the main module.
	PyObject* main_module = PyImport_AddModule("__main__");

	// Get the main module's dictionary.
	PyObject* main_dict = PyModule_GetDict(main_module);
	// Execute two different files of Python code in separate environments
	FILE* file = fopen("client.py", "r");
	PyRun_File(file, "client.py", Py_file_input, main_dict, main_dict);

    Py_Finalize();
    return 0;
}