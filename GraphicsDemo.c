/**

 GraphicsLib.c

**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "Protocol/Shell.h"
#include "GraphicsLib/Graphics.h"
#include "Rand.h"

STATIC VOID WaitKeyPress(VOID);

#define RAND_COLOUR (UINT32)(Rand() % 0x1000000)

CHAR16 *Story[] = {
    L"The Three Little Pigs\n",
    L"=====================\n",
    L"\n",
    L"Once upon a time there were three little pigs.\nOne pig built a house of straw while the second pig built his house with sticks.\n",
    L"They built their houses very quickly and then sang and danced all day because they were lazy.\n",
    L"The third little pig worked hard all day and built his house with bricks.\n",
    L"A big bad wolf saw the two little pigs while they danced and played and thought, What juicy tender meals they will make!\n",
    L"He chased the two pigs and they ran and hid in their houses.\n",
    L"The big bad wolf went to the first house and huffed and puffed and blew the house down in minutes.\n",
    L"The frightened little pig ran to the second pig’s house that was made of sticks.\n",
    L"The big bad wolf now came to this house and huffed and puffed and blew the house down in hardly any time.\n",
    L"Now, the two little pigs were terrified and ran to the third pig’s house that was made of bricks.\n",
    L"The big bad wolf tried to huff and puff and blow the house down, but he could not.\n",
    L"He kept trying for hours but the house was very strong and the little pigs were safe inside.\n",
    L"He tried to enter through the chimney but the third little pig boiled a big pot of water and kept it below the chimney.\n",
    L"The wolf fell into it and died.\n",
    L"The two little pigs now felt sorry for having been so lazy.\n",
    L"They too built their houses with bricks and lived happily ever after.\n",
    NULL
};

INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
    EFI_STATUS Status = EFI_SUCCESS;

    // Initialise graphics library
    Status = InitGraphics();
    if (EFI_ERROR(Status)) {
        Print(L"ERROR: Failed to initialise Graphics Library (%r)\n", Status);
        goto Error_exit;
    }

    // List available graphics mode
    Print(L"Available graphics modes:\n");
    for (UINTN i=0; i<NumGraphicsModes(); i++) {
        UINT32 HorRes, VerRes;
        if (!EFI_ERROR(QueryGraphicsMode(i, &HorRes, &VerRes))) {
            Print(L"%2u: %ux%u\n", i , HorRes, VerRes);
        }
    }
    Print(L"\nPress any key to continue\n");
    WaitKeyPress();
    gST->ConOut->ClearScreen (gST->ConOut); 
    
    // Set our prefered display resolution
    Status = SetDisplayResolution(1024, 768);
    if (EFI_ERROR(Status)) {
        Print(L"ERROR: Failed to set display resolution (%r)\n", Status);
        goto Error_exit;
    }

    // Read current graphics mode
    UINTN Mode;
    Status = GetGraphicsMode(&Mode);
    if (EFI_ERROR(Status)) {
        RestoreConsole();
        Print(L"ERROR: Failed to read current graphics mode (%r)\n", Status);
        goto Error_exit;
    }

    // Read current screen resolution
    UINT32 DisplayWidth = GetFBHorRes();
    UINT32 DisplayHeight = GetFBVerRes();

    // Display current graphics info
    GPrint(NULL, L"Graphics mode: %d\n", Mode);
    GPrint(NULL, L"Resolution   : %d x %d\n", DisplayWidth, DisplayHeight);
    GPrint(NULL, L"\nPress any key to continue\n");
    WaitKeyPress();

    // Draw pixels
    ClearScreen(BLACK);
    for (UINTN i=0; i<1000; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        PutPixel(x0, y0, RAND_COLOUR);
    }
    GPutString(0, 0, L"PIXELS", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Draw lines
    ClearScreen(BLACK);
    for (UINTN i=0; i<100; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        INT32 x1 = Rand() % DisplayWidth;
        INT32 y1 = Rand() % DisplayHeight;
        DrawLine(x0, y0, x1, y1, RAND_COLOUR);
    }
    GPutString(0, 0, L"LINES", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Draw horizontal lines
    ClearScreen(BLACK);
    for (UINTN i=0; i<100; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 x1 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        UINT32 w = ABS(x0-x1);
        DrawHLine((x0 > x1) ? x1 : x0, y0, w, RAND_COLOUR);
    }
    GPutString(0, 0, L"HORIZONTIAL LINES", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Draw vertical lines
    ClearScreen(BLACK);
    for (UINTN i=0; i<100; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        INT32 y1 = Rand() % DisplayHeight;
        UINT32 h = ABS(y0-y1);
        DrawVLine(x0, (y0 > y1) ? y1 : y0, h, RAND_COLOUR);
    }
    GPutString(0, 0, L"VERTICAL LINES", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Draw rectangles
    ClearScreen(BLACK);
    for (UINTN i=0; i<20; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        INT32 x1 = Rand() % DisplayWidth;
        INT32 y1 = Rand() % DisplayHeight;
        DrawRectangle(x0, y0, x1, y1, RAND_COLOUR);
    }
    GPutString(0, 0, L"RECTANGLES", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Draw filled rectangles
    ClearScreen(BLACK);
    for (UINTN i=0; i<20; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        INT32 x1 = Rand() % DisplayWidth;
        INT32 y1 = Rand() % DisplayHeight;
        DrawFillRectangle(x0, y0, x1, y1, RAND_COLOUR);
    }
    GPutString(0, 0, L"FILLED RECTANGLES", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Draw circles
    ClearScreen(BLACK);
    for (UINTN i=0; i<20; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        INT32 dist1 = x0 > DisplayWidth-x0 ? DisplayWidth-x0 : x0;
        INT32 dist2 = y0 > DisplayHeight-y0 ? DisplayHeight-y0 : y0;
        INT32 r = Rand() % (dist1 > dist2 ? dist2 : dist1);
        DrawCircle(x0, y0, r, RAND_COLOUR);
    }
    GPutString(0, 0, L"CIRCLES", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Draw filled circles
    ClearScreen(BLACK);
    for (UINTN i=0; i<20; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        INT32 dist1 = x0 > DisplayWidth-x0 ? DisplayWidth-x0 : x0;
        INT32 dist2 = y0 > DisplayHeight-y0 ? DisplayHeight-y0 : y0;
        INT32 r = Rand() % (dist1 > dist2 ? dist2 : dist1);
        DrawFillCircle(x0, y0, r, RAND_COLOUR);
    }
    GPutString(0, 0, L"FILLED CIRCLES", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Drawing in a clipped region
    ClearScreen(BLACK);
    GPutString(0, 0, L"CLIPPED REGION", WHITE, BLACK, TRUE, FONT10x20);
    SetClipping(200, 100, DisplayWidth-200, DisplayHeight-100);
    for (UINTN i=0; i<200; i++) {
        INT32 x0 = Rand() % DisplayWidth;
        INT32 y0 = Rand() % DisplayHeight;
        INT32 x1 = Rand() % DisplayWidth;
        INT32 y1 = Rand() % DisplayHeight;
        DrawLine(x0, y0, x1, y1, RAND_COLOUR);
    }
    ResetClipping();
    WaitKeyPress();

    // Render buffer
    ClearScreen(BLACK);
    {
        // Create off line buffer to hold image
        RENDER_BUFFER RenBuf;
        INT32 Radius = 100;
        INT32 PixSize = (2*Radius + 1) + 2;
        Status = CreateRenderBuffer(&RenBuf, PixSize, PixSize);
        if (EFI_ERROR(Status)) {
            Print(L"ERROR: Failed to create render buffer (%r)\n", Status);
            goto Error_exit;
        }
        // set off line buffer to be target for all drawing
        SetRenderBuffer(&RenBuf);
        // Draw sphere to off line buffer
        for (INT32 i=Radius; i>=0; i--) {
            DrawFillCircle(Radius+1, Radius+1, i, RGB_COLOUR(0, 255-((200*i + Radius/2)/Radius), 0));
        }            
        // Set screen as target for all drawing
        SetScreenRender();
        // Draw off buffer to screen multiple times
        for (INT32 x=0; x<DisplayWidth; x+=2*Radius) {
            for (INT32 y=0; y<DisplayHeight; y+=2*Radius) {
                DisplayRenderBuffer(&RenBuf, x, y);
            }
        }
    }
    GPutString(0, 0, L"RENDER BUFFER", WHITE, BLACK, TRUE, FONT10x20);
    WaitKeyPress();

    // Display text
    ClearScreen(BLACK);
    GPrint(NULL, L"TEXT\n\n");
    {
        // Print story to text box
        UINTN i = 0;
        while (Story[i]) {
            GPrint(NULL, Story[i++]);
        }
    }
    WaitKeyPress();

    // Display text box
    ClearScreen(BLACK);
    GPutString(0, 0, L"TEXT BOX", WHITE, BLACK, TRUE, FONT10x20);
    {
        // Create text box
        STATIC TEXT_CONFIG TxtCfg = {0};
        CreateTextBox(&TxtCfg,300, 200, DisplayWidth-600, DisplayHeight-400, BLACK, LIGHT_GRAY, FONT8x13B);
        // Draw surrounding rectangle
        DrawRectangle(299, 199, DisplayWidth-299, DisplayHeight-199, MAGENTA);
        // Clear text box
        ClearTextBox(&TxtCfg);
        // Print story to text box
        UINTN i = 0;
        while (Story[i]) {
            GPrint(&TxtCfg, Story[i++]);
            gBS->Stall(100000);
        }

    }
    WaitKeyPress();

    // Display all supported fonts
    ClearScreen(BLACK);
    GPutString(0, 0, L"FONTS", WHITE, BLACK, TRUE, FONT10x20);
    {
        INT32 y=50;
        for (UINTN i=0 ; i<FontListSize; i++) {
            GPutString(0, y, L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrsuvwxyz{|}~",
                        WHITE, BLACK, TRUE, (FONT) FontList[i]);
            y += GetFontHeight((FONT)FontList[i]) + 5;

        }
    }
    WaitKeyPress();

    // Restore console prior to exiting
    RestoreConsole();

Error_exit:

    return SHELL_SUCCESS;
}


/*
 * WaitKeyPress()
 */
STATIC VOID WaitKeyPress(VOID)
{
    EFI_INPUT_KEY key;
    UINTN index;
    // flush keyboard
    while (!EFI_ERROR(gST->ConIn->ReadKeyStroke(gST->ConIn, &key)));
    // wait for key
    if (!EFI_ERROR(gST->BootServices->WaitForEvent(1, &gST->ConIn->WaitForKey, &index))) {
        // read key pressed
        while (!EFI_ERROR(gST->ConIn->ReadKeyStroke(gST->ConIn, &key)));
    }
}
