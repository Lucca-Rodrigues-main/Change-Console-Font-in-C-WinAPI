#include <stdio.h>
#include <locale.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI GetCurrentConsoleFont(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFO lpConsoleCurrentFont);

typedef struct _CONSOLE_FONT_INFOEX {
  ULONG cbSize;
  DWORD nFont;
  COORD dwFontSize;
  UINT  FontFamily;
  UINT  FontWeight;
  WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
BOOL WINAPI GetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);

typedef struct _CONSOLE_SCREEN_BUFFER_INFOEX {
    ULONG      cbSize;
    COORD      dwSize;
    COORD      dwCursorPosition;
    WORD       wAttributes;
    SMALL_RECT srWindow;
    COORD      dwMaximumWindowSize;
    WORD       wPopupAttributes;
    BOOL       bFullscreenSupported;
    COLORREF   ColorTable[16];
} CONSOLE_SCREEN_BUFFER_INFOEX, *PCONSOLE_SCREEN_BUFFER_INFOEX;
BOOL WINAPI GetConsoleScreenBufferInfoEx(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx);

BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

typedef struct
{
	int cp;
	int ocp;
	char local[40];
	DWORD console_mode;
	FILE* log;
	CONSOLE_FONT_INFO font_info;
	CONSOLE_FONT_INFOEX font_infoEx;
	CONSOLE_SCREEN_BUFFER_INFO info;
	CONSOLE_SCREEN_BUFFER_INFOEX infoEx;
}  FONTE;

int GuardaFonte(FONTE* antes); // Armazena as configurações da fonte atual em uma estrutura do tipo FONTE
int AlteraFonte(FONTE* novo); // Armazena as configurações da fonte atual em uma estrutura do tipo FONTE
int RestauraFonte(FONTE* antes); // Restaura as configurações da fonte armazenadas anteriormente com GuardaFonte()

int main()
{
    FONTE original;

    // Guarda configuracao original
    GuardaFonte(&original);

    // Prepara nova configuracao (Consolas 8x16)
    FONTE nova = original;
    nova.cp = 437;
    nova.ocp = 437;
    nova.font_infoEx.nFont = 0;
    nova.font_infoEx.dwFontSize.X = 8;
    nova.font_infoEx.dwFontSize.Y = 16;
    wcscpy(nova.font_infoEx.FaceName, L"Consolas");
    nova.font_infoEx.FontFamily = 54;
    nova.font_infoEx.FontWeight = 400;

    // Aplica nova fonte
    AlteraFonte(&nova);
    printf("Esta frase deve aparecer em Consolas 8x16\n");

    getchar();

    // Restaura configuracao original
    RestauraFonte(&original);
    printf("Fonte restaurada para o padrao\n");

    return 0;
}

int GuardaFonte(FONTE* antes)
{
	antes->cp = GetConsoleCP();
	antes->ocp = GetConsoleOutputCP();
	strcpy(antes->local, setlocale(LC_ALL, NULL));
	HANDLE  h = GetStdHandle(STD_OUTPUT_HANDLE);
    int res = 0;

	res = GetCurrentConsoleFont(h, 0, &(antes->font_info));
	if (res == 0){
	    // GetCurrentConsoleFont() falhou
	    return 1;
	}
	antes->font_infoEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	res = GetCurrentConsoleFontEx(h, 0, &(antes->font_infoEx));
	if (res == 0){
	    // GetCurrentConsoleFontEx() falhou
	    return 1;
	}
	/* A fonte em uso fica no registro
    Computador\HKEY_LOCAL_MACHINE\SOFTWARE\
    Microsoft\Windows NT\CurrentVersion\Console\TrueTypeFont */

	GetConsoleMode(h, &antes->console_mode);
	// Agora falta SCREEN BUFFER
	antes->infoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(h, &antes->infoEx);

	return 0;
}

int AlteraFonte(FONTE* novo)
{
    // Restaura locale e páginas de código
	SetConsoleCP(novo->cp);
	SetConsoleOutputCP(novo->ocp);
	setlocale(LC_ALL, novo->local);

	// Restaura a fonte
	HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
	int res = SetCurrentConsoleFontEx(ho, 0, &novo->font_infoEx);
	if (res == 0){
	    // SetCurrentConsoleFontEx() falhou
	    return 1;
	}
	// Console mode
	SetConsoleMode(ho, novo->console_mode);
	// Cor e fundo
	SetConsoleTextAttribute(ho, novo->infoEx.wAttributes);

	return 0;
}

int RestauraFonte(FONTE* antes)
{
	// Restaura locale e páginas de código
	SetConsoleCP(antes->cp);
	SetConsoleOutputCP(antes->ocp);
	setlocale(LC_ALL, antes->local);

	// Restaura a fonte
	HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
	int res = SetCurrentConsoleFontEx(ho, 0, &antes->font_infoEx);
	if (res == 0){
	    // SetCurrentConsoleFontEx() falhou
	    return 1;
	}
	// Console mode
	SetConsoleMode(ho, antes->console_mode);
	// Cor e fundo
	SetConsoleTextAttribute(ho, antes->infoEx.wAttributes);

	return 0;
}
