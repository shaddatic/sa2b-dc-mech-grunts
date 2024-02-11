#include <sa2b/core.h>
#include <sa2b/init.h>
#include <sa2b/memutil.h>
#include <sa2b/config.h>

/** Ninja **/
#include <sa2b/ninja/ninja.h>

/** Game **/
#include <sa2b/src/task.h>
#include <sa2b/src/sound.h>

static bool TWalkSw;
static bool EWalkSw;

void __cdecl
SE_Rand(int32_t se, NJS_VECTOR* pPos, TASK* pTask, int8_t pri, int8_t Volume)
{
    if (se == 0x3000) // Tails
    {
        if (!TWalkSw)
            goto NO_INC;
    }
    else if (se == 0x300A)
    {
        if (!EWalkSw)
            goto NO_INC;
    }
    else
        goto NO_INC;

    if (njRandom() < 0.5f)
        ++se;

NO_INC:
    SE_CallV2(se, pTask, pPos,  pri, Volume);
}

__declspec(naked)
static void
__SE_Rand()
{
    __asm
    {
        push[esp + 0Ch]
        push[esp + 0Ch]
        push[esp + 0Ch]
        push esi
        push edi

        call SE_Rand

        pop edi
        pop esi
        add esp, 12
        retn
    }
}

EXPORT_DLL
void __cdecl
Init(const char* path, const HelperFunctions* pHelpFuncs)
{
    config* const p_config = ConfigOpen2(path, "config.ini");

    TWalkSw = ConfigGetInt(p_config, "main", "twalk_sw", 1);
    EWalkSw = ConfigGetInt(p_config, "main", "ewalk_sw", 1);

    WriteCall(0x007492DA, __SE_Rand); // SoundSwitch 1
    WriteCall(0x007493EE, __SE_Rand); // SoundSwitch 2

    if (EWalkSw)
        WriteCall(0x0073C69F, __SE_Rand); // Eggman

    ConfigClose(p_config);
}

EXPORT_DLL
ModInfo SA2ModInfo = { MODLOADER_VER };
