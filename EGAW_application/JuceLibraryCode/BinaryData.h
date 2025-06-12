/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   EGAW_icon_png;
    const int            EGAW_icon_pngSize = 10133;

    extern const char*   EGAW_main_png;
    const int            EGAW_main_pngSize = 34572;

    extern const char*   EGAW_title_png;
    const int            EGAW_title_pngSize = 27715;

    extern const char*   eyeClosed_svg;
    const int            eyeClosed_svgSize = 1673;

    extern const char*   eyeOpen_svg;
    const int            eyeOpen_svgSize = 978;

    extern const char*   deleteCursor_png;
    const int            deleteCursor_pngSize = 33685;

    extern const char*   bypass_svg;
    const int            bypass_svgSize = 557;

    extern const char*   delete_icon_png;
    const int            delete_icon_pngSize = 684;

    extern const char*   palette_icon_png;
    const int            palette_icon_pngSize = 10791;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 9;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
