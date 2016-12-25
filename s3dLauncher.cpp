#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <windows.h>
#include <Shellapi.h>

static char const* factory = "C:\\Users\\lordjaxom\\AppData\\Local\\Simplify3D\\S3D-Software\\savedState.factory";
static char const* profile = "Replicator.fff";
static char const* executable = "Simplify3D.exe";

using namespace std;

int main()
{
	DeleteFileA( factory );

	ifstream ifs( profile, ios::in );

	string contents;
	char c;
	while ( ifs && !ifs.get( c ).eof() ) {
		if ( c == '\n' || c == '\r' ) {
			continue;
		}
		contents.push_back( c );
	}
	contents.push_back( '\0' );
	contents.push_back( '\0' );

	if ( ifs.bad() ) {
		MessageBoxA( 0, "Error reading profile", "Error", MB_OK | MB_ICONERROR );
		return EXIT_FAILURE;
	}
    
    HKEY hKey = 0;
	if ( RegOpenKeyExA( HKEY_CURRENT_USER, "SOFTWARE\\Simplify3D\\S3D-Software\\FFFWindow", 0, KEY_ALL_ACCESS, &hKey ) != ERROR_SUCCESS ||
			RegSetValueExA( hKey, "profileDatabaseContents", 0, REG_MULTI_SZ, (BYTE*) contents.data(), contents.size() ) != ERROR_SUCCESS ||
			RegSetValueExA( hKey, "profileDatabaseNames", 0, REG_MULTI_SZ, (BYTE*) "Replicator\0\0", 12 ) != ERROR_SUCCESS ) {
		MessageBoxA( 0, "Error modifying registry", "Error", MB_OK | MB_ICONERROR );
		return EXIT_FAILURE;
	}
	RegCloseKey( hKey );

	//if ( !CreateProcessA( 0, simplify3d, 0, 0, FALSE, 0, 0, 0, 0, 0 ) ) {
	if ( (uintmax_t) ShellExecuteA( 0, "open", executable, 0, 0, SW_SHOWDEFAULT ) <= 32 ) {
		MessageBoxA( 0, "Error starting Simplify3D", "Error", MB_OK | MB_ICONERROR );
		return EXIT_FAILURE;
	}
}

