/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== TODO.md ==================
static const unsigned char temp_binary_data_0[] =
"#  TODO\n"
"\n"
"\n"
"\n"
"# DONE\n"
"\n"
"fix rpath:\n"
"install_name_tool -change /usr/local/lib/libsndfile.1.dylib @rpath/libsndfile.1.dylib LinearPredictionCoder\n"
"\n"
"fine slider (cents?) for sub-semitone adjustments.\n";

const char* TODO_md = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xe63de430:  numBytes = 190; return TODO_md;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "TODO_md"
};

const char* originalFilenames[] =
{
    "TODO.md"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
