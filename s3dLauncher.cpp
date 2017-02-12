#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include <windows.h>
#include <Shellapi.h>

static char const* factory = "C:\\Users\\lordjaxom\\AppData\\Local\\Simplify3D\\S3D-Software\\savedState.factory";
static char const* profiles[] = { "Replicator.fff", "Ultimaker.fff" };
static char const* executable = "Simplify3D.exe";

using namespace std;

std::string readFileSkipNewlines( char const* path )
{
	ifstream ifs( path, ios::in );
	string result;
	char c;
	while ( ifs && !ifs.get( c ).eof() ) {
		if ( c == '\n' || c == '\r' ) {
			continue;
		}
		result.push_back( c );
	}
	if ( ifs.bad() ) {
		std::string message = std::string( "Error reading profile " ) + path;
		MessageBoxA( 0, message.c_str(), "Error", MB_OK | MB_ICONERROR );
		exit( EXIT_FAILURE );
	}
	return result;
}

int main()
{
	DeleteFileA( factory );

	string contents;
	for ( size_t i = 0 ; i < sizeof( profiles ) / sizeof( profiles[ 0 ] ) ; ++i ) {
		contents += readFileSkipNewlines( profiles[ i ] ) + '\0';
	}
	contents += '\0';

    	HKEY hKey = 0;
	if ( RegOpenKeyExA( HKEY_CURRENT_USER, "SOFTWARE\\Simplify3D\\S3D-Software\\FFFWindow", 0, KEY_ALL_ACCESS, &hKey ) != ERROR_SUCCESS ||
			RegSetValueExA( hKey, "profileDatabaseContents", 0, REG_MULTI_SZ, (BYTE*) contents.data(), contents.size() ) != ERROR_SUCCESS ||
			RegSetValueExA( hKey, "profileDatabaseNames", 0, REG_MULTI_SZ, (BYTE*) "Replicator\0Ultimaker\0\0", 22 ) != ERROR_SUCCESS ) {
		MessageBoxA( 0, "Error writing FFF-Database contents to registry", "Error", MB_OK | MB_ICONERROR );
		return EXIT_FAILURE;
	}
	RegCloseKey( hKey );
	hKey = 0;
	if ( RegOpenKeyExA( HKEY_CURRENT_USER, "SOFTWARE\\Simplify3D\\S3D-Software\\FFF", 0, KEY_ALL_ACCESS, &hKey ) != ERROR_SUCCESS ) {
		MessageBoxA( 0, "Error modifying registry", "Error", MB_OK | MB_ICONERROR );
		return EXIT_FAILURE;
	}
	RegDeleteValueA( hKey, "profileName" );
	RegDeleteValueA( hKey, "profileVersion" );
	RegCloseKey( hKey );

	if ( (uintmax_t) ShellExecuteA( 0, "open", executable, 0, 0, SW_SHOWDEFAULT ) <= 32 ) {
		MessageBoxA( 0, "Error starting Simplify3D", "Error", MB_OK | MB_ICONERROR );
		return EXIT_FAILURE;
	}
}

