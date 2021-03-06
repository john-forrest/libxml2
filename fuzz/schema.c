/*
 * schema.c: a libFuzzer target to test the XML Schema processor.
 *
 * See Copyright for the status of this software.
 */

#include <libxml/xmlschemas.h>
#include "fuzz.h"

int
LLVMFuzzerInitialize(int *argc ATTRIBUTE_UNUSED,
                     char ***argv ATTRIBUTE_UNUSED) {
    xmlInitParser();
    xmlSetGenericErrorFunc(NULL, xmlFuzzErrorFunc);
    xmlSetExternalEntityLoader(xmlFuzzEntityLoader);

    return 0;
}

int
LLVMFuzzerTestOneInput(const char *data, size_t size) {
    xmlSchemaParserCtxtPtr pctxt;

    xmlFuzzDataInit(data, size);
    xmlFuzzReadEntities();

    pctxt = xmlSchemaNewParserCtxt(xmlFuzzMainUrl());
    xmlSchemaSetParserErrors(pctxt, xmlFuzzErrorFunc, xmlFuzzErrorFunc, NULL);
    xmlSchemaFree(xmlSchemaParse(pctxt));
    xmlSchemaFreeParserCtxt(pctxt);

    xmlFuzzDataCleanup();

    return(0);
}

