
#include <stdio.h>
#include </usr/include/python2.7/Python.h>
#include </usr/include/python2.7/pyhelper.hpp>

int main()
{
	CPyInstance hInstance;

	CPyObject pName = PyUnicode_FromString("externalPy.py");
	CPyObject pModule = PyImport_Import(pName);

	if(pModule)
	{
		CPyObject pFunc = PyObject_GetAttrString(pModule, "getInteger");
		if(pFunc && PyCallable_Check(pFunc))
		{
			CPyObject pValue = PyObject_CallObject(pFunc, NULL);

			printf_s("C: getInteger() = %ld\n", PyLong_AsLong(pValue));
		}
		else
		{
			printf("ERROR: function getInteger()\n");
		}

	}
	else
	{
		printf_s("ERROR: Module not imported\n");
	}

	return 0;
}