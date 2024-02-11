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

static bool Random;

static void __cdecl
SE_Switch(int32_t se, NJS_VECTOR* pPos, TASK* pTask, int8_t pri, int8_t Volume)
{
    if ((se == 0x3000 && TWalkSw) || (se == 0x300A && EWalkSw))
    {
        if (Random)
        {
            if (njRandom() < 0.5f)
                ++se;
        }
        else
        {
            if (njScalor(&pTask->mwp->spd) > 10.0f)
                ++se;
        }
    }

    SE_CallV2(se, pTask, pPos,  pri, Volume);
}

__declspec(naked)
static void
__SE_Switch()
{
    __asm
    {
        push[esp + 0Ch]
        push[esp + 0Ch]
        push[esp + 0Ch]
        push esi
        push edi

        call SE_Switch

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

    Random = ConfigGetInt(p_config, "main", "random", 0);

    WriteCall(0x007492DA, __SE_Switch);
    WriteCall(0x007493EE, __SE_Switch);

    ConfigClose(p_config);
}

EXPORT_DLL
ModInfo SA2ModInfo = { MODLOADER_VER };
