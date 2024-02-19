#include "MainScript.hpp"
#include "ScriptFiber.hpp"
#include "Natives.hpp"
#include "Timer.hpp"
#include "CustomText.hpp"
#include "UI/UIManager.hpp"
#include "UI/BoolOption.hpp"
#include "UI/BreakOption.hpp"
#include "UI/ChooseBoolOption.hpp"
#include "UI/ChooseOption.hpp"
#include "UI/KeyboardOption.hpp"
#include "UI/NumberBoolOption.hpp"
#include "UI/NumberOption.hpp"
#include "UI/RegularOption.hpp"
#include "UI/SubOption.hpp"
#include "UI/RegularSubmenu.hpp"
#include "UI/PlayerSubmenu.hpp"
#include "Features/Player.hpp"
#include "Features/Weapon.hpp"
#include "Features/Vehicle.hpp"
#include "Features/Network.hpp"
#include "Features/Protections.hpp"
#include "Features/World.hpp"
#include "Features/Misc.hpp"
#include "Features/Teleport.hpp"
#include "Features/Functions.hpp"
#include "Game.hpp"
#include "Removals.hpp"
#include "enums.hpp"
#include "ImGui/imgui.h"
#include "Memory/Module.hpp"
#include "Invoker.hpp"

// TODO:
// Translate this entire thing

namespace Sentinel
{
	enum Submenu : std::uint32_t
	{
		SubmenuHome,
		SubmenuPlayer,
		SubmenuPedFlags,
		SubmenuPedProofs,
		SubmenuMovement,
		SubmenuHandtrails,
		SubmenuWeapon,
		SubmenuGunLocker,
		SubmenuMeleeWeapons,
		SubmenuHandguns,
		SubmenuSMGs,
		SubmenuShotguns,
		SubmenuAssaultRifles,
		SubmenuLightMachineGuns,
		SubmenuSniperRifles,
		SubmenuHeavyWeapons,
		SubmenuThrowables,
		SubmenuMiscellaneousWeapons,
		SubmenuModifiers,
		SubmenuPaintGun,
		SubmenuImpact,
		SubmenuVehicle,
		SubmenuHandling,
		SubmenuSpawner,
		SubmenuSpawnSettings,
		SubmenuVehicleSpawner,
		SubmenuNetwork,
		SubmenuRIDToolkit,
		SubmenuSessionManagement,
		SubmenuESP,
		SubmenuProtections,
		SubmenuPlayerList,
		SubmenuSelectedPlayer,
		SubmenuRemovals,
		SubmenuRemote,
		SubmenuSocialClub,
		SubmenuNTeleport,
		SubmenuGriefing,
		SubmenuMoneyDrop,
		SubmenuSessionStarter,
		SubmenuWorld,
		SubmenuTime,
		SubmenuWeather,
		SubmenuClouds,
		SubmenuGlow,
		SubmenuMisc,
		SubmenuTeleport,
		SubmenuRecovery,
		SubmenuMoney,
		SubmenuRank,
		SubmenuUnlocks,
		SubmenuSettings
	};

	void MainScript::Initialize()
	{
		using namespace UserInterface;

		g_CustomText->AddText(RAGE_JOAAT("HUD_JOINING"), "Loading GTA Online with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("HUD_QUITTING"), "Leaving with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("PM_QUIT_MP"), "Exiting GTA Online with " SENTINEL_NAME);
		g_CustomText->AddText(RAGE_JOAAT("PM_ENTER_MP"), "Play GTA Online with " SENTINEL_NAME);
		g_CustomText->AddText(RAGE_JOAAT("PM_EXIT_GAME"), "Quit GTA with" SENTINEL_NAME);
		g_CustomText->AddText(RAGE_JOAAT("PM_GO"), "Freemode with " SENTINEL_NAME);
		g_CustomText->AddText(RAGE_JOAAT("HUD_MPREENTER"), "Joining a new session with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("PM_FRIEND_FM"), "Closed Friend Session with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("PM_GOTO_STORE"), "Purchase Shark Cards with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("PM_INF_EXIT"), "Quit to desktop with " SENTINEL_NAME "?");
		g_CustomText->AddText(RAGE_JOAAT("PM_CRESES"), "Joining A Crew Only Session with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("PM_INF_PGOB"), "Cut to the chase and	head straight into the world of GTA Online with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("PM_INF_PGOT"), "Online w/ " SENTINEL_NAME "!");
		g_CustomText->AddText(RAGE_JOAAT("PM_INVO_FM"), "Join A Invite Only Session with " SENTINEL_NAME ".");
		g_CustomText->AddText(RAGE_JOAAT("LOADING_SPLAYER_L"), "Loading Story Mode with " SENTINEL_NAME "...");
		g_CustomText->AddText(RAGE_JOAAT("PM_NCREW_FM"), "Join A Crew Session with " SENTINEL_NAME ".");
		g_CustomText->AddText(RAGE_JOAAT("PM_CREW_FM"), "Join A Closed Crew Session with " SENTINEL_NAME ".");
		g_CustomText->AddText(RAGE_JOAAT("PM_SOLO_FM"), "Join A Solo Session with " SENTINEL_NAME ".");
		g_CustomText->AddText(RAGE_JOAAT("PM_FRESES"), "Join A Friends Only Session with " SENTINEL_NAME ".");
		g_CustomText->AddText(RAGE_JOAAT("HUD_MAINTIT"), SENTINEL_NAME);
		g_CustomText->AddText(RAGE_JOAAT("MP_CHAT_ALL"), SENTINEL_NAME);
		g_CustomText->AddText(RAGE_JOAAT("GC_OTR_TMR"), "HIDING FROM SKIDS");

		g_UiManager->AddSubmenu<RegularSubmenu>("Lite V2.1", SubmenuHome, [](RegularSubmenu* sub)
		{
			g_Functions->m_VehicleSpawnerOpen = false;
			sub->AddOption<SubOption>("Player", nullptr, SubmenuPlayer);
			sub->AddOption<SubOption>("Weapon", nullptr, SubmenuWeapon);
			sub->AddOption<SubOption>("Vehicle", nullptr, SubmenuVehicle);
			sub->AddOption<SubOption>("Spawner", nullptr, SubmenuSpawner);
			sub->AddOption<SubOption>("Network", nullptr, SubmenuNetwork);
			sub->AddOption<SubOption>("World", nullptr, SubmenuWorld);
			sub->AddOption<SubOption>("Miscellaneous", nullptr, SubmenuMisc);
			sub->AddOption<SubOption>("Teleport", nullptr, SubmenuTeleport);
			sub->AddOption<SubOption>("Recovery", nullptr, SubmenuRecovery);
			sub->AddOption<SubOption>("Settings", nullptr, SubmenuSettings);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Player", SubmenuPlayer, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Ped Flags", nullptr, SubmenuPedFlags);
			sub->AddOption<SubOption>("Ped Proofs", nullptr, SubmenuPedProofs);
			sub->AddOption<SubOption>("Movement", nullptr, SubmenuMovement);
			sub->AddOption<SubOption>("Handtrails", nullptr, SubmenuHandtrails);
			sub->AddOption<BoolOption<bool>>("God", nullptr, &g_PlayerOptions->m_God, [] 
			{  
				QUEUE()
				{
					if (!g_PlayerOptions->m_God)
						ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), FALSE);
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Semi God", nullptr, &g_PlayerOptions->m_SemiGod);
			sub->AddOption<BoolOption<bool>>("No Ragdoll", nullptr, &g_PlayerOptions->m_NoRagdoll, []
			{
				QUEUE()
				{
					if (!g_PlayerOptions->m_NoRagdoll) {
						for (const auto& Flag : g_RagdollBlockingFlags)
							PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(PLAYER::PLAYER_PED_ID(), static_cast<int>(Flag));

						PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), TRUE);
						PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), TRUE);
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 287, FALSE);

						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 166, TRUE);
						PED::SET_PED_INJURED_ON_GROUND_BEHAVIOUR(PLAYER::PLAYER_PED_ID(), NULL);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("No Cops", nullptr, &g_PlayerOptions->m_NoCops, []
			{
				if (!g_PlayerOptions->m_NoCops) {
					g_Pointers->m_MaxWantedLevel->Restore();
					g_Pointers->m_MaxWantedLevel2->Restore();
				}
			});
			sub->AddOption<ChooseBoolOption<const char*, std::size_t, bool>>("Invisibility", nullptr, &g_PlayerOptions->m_Invisibility, &g_PlayerOptions->m_InvisibilityList, &g_PlayerOptions->m_InvisibilityListPos, true, []
			{
				QUEUE()
				{
					if (!g_PlayerOptions->m_Invisibility)
					{
						ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
						NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(PLAYER::PLAYER_PED_ID(), FALSE);
						NETWORK::SET_LOCAL_PLAYER_INVISIBLE_LOCALLY(FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Infinite Ability", nullptr, &g_PlayerOptions->m_InfiniteAbility);
			sub->AddOption<BoolOption<bool>>("Infinite Stamina", nullptr, &g_PlayerOptions->m_InfiniteStamina);
			sub->AddOption<BoolOption<bool>>("Ignored", nullptr, &g_PlayerOptions->m_Ignored);
			sub->AddOption<BoolOption<bool>>("Off Radar", nullptr, &g_PlayerOptions->m_OffRadar, []
			{
				if (!g_PlayerOptions->m_OffRadar) {
					*ScriptGlobal(2657589).Add(PLAYER::PLAYER_ID(), 466).Add(210).As<std::int32_t*>() = 0;
					*ScriptGlobal(2672505).Add(57).As<std::int32_t*>() = 0;
				}
			});
			sub->AddOption<BoolOption<bool>>("Cops Turn Blind Eye", nullptr, &g_PlayerOptions->m_CopsTurnBlindEye, []
			{
				if (!g_PlayerOptions->m_CopsTurnBlindEye) {
					*ScriptGlobal(2793046).Add(4654).As<std::int32_t*>() = 0;
					*ScriptGlobal(2793046).Add(4657).As<std::int32_t*>() = 0;
				}
			});
			sub->AddOption<BoolOption<bool>>("Reveal Players", nullptr, &g_PlayerOptions->m_RevealPlayers, []
			{
				if (!g_PlayerOptions->m_RevealPlayers) {
					*ScriptGlobal(2657589).Add(PLAYER::PLAYER_ID(), 466).Add(213).As<std::int32_t*>() = 0;
					*ScriptGlobal(2672505).Add(58).As<std::int32_t*>() = 0;
				}
			});
			sub->AddOption<BoolOption<bool>>("Night Vision", nullptr, &g_PlayerOptions->m_NightVision, []
			{
				QUEUE()
				{
					if (!g_PlayerOptions->m_NightVision) {
						*ScriptGlobal(1853910).Add(PLAYER::PLAYER_ID(), 862).Add(821).Add(11).As<int*>() = 0;
						GRAPHICS::SET_NIGHTVISION(FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Thermal Vision", nullptr, &g_PlayerOptions->m_ThermalVision, []
			{
				QUEUE()
				{
					if (!g_PlayerOptions->m_ThermalVision) {
						*ScriptGlobal(1853910).Add(PLAYER::PLAYER_ID(), 862).Add(821).Add(11).As<int*>() = 0;
						GRAPHICS::SET_SEETHROUGH(FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Karma", nullptr, &g_PlayerOptions->m_Karma);
			sub->AddOption<BoolOption<bool>>("Spin Bot", nullptr, &g_PlayerOptions->m_SpinBot);
			sub->AddOption<RegularOption>("Suicide", nullptr, []
			{
				Game::GetLocalPed()->m_health = 0;
				g_Notifications->Push("Suicide", "We've set your health to zero.");
			});
			sub->AddOption<RegularOption>("Heal", nullptr, []
			{
				Game::GetLocalPed()->m_health = Game::GetLocalPed()->m_maxhealth;
				g_Notifications->Push("Heal", "We've restored your health.");
			});
			sub->AddOption<RegularOption>("Remove Armor", nullptr, []
			{
				Game::GetLocalPed()->m_armor = 0;
				g_Notifications->Push("Remove Armor", "We've destroyed your armor.");
			});
			sub->AddOption<RegularOption>("Repair Armor", nullptr, []
			{
				QUEUE() { Game::GetLocalPed()->m_armor = PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID()); } STOP
				g_Notifications->Push("Repair Armor", "We've repaired your armor.");
			});
			sub->AddOption<RegularOption>("Clear Visual Damage", nullptr, []
			{
				QUEUE()
				{
					auto Ped = PLAYER::PLAYER_PED_ID();
					PED::CLEAR_PED_BLOOD_DAMAGE(Ped);
					PED::CLEAR_PED_WETNESS(Ped);
					PED::CLEAR_PED_ENV_DIRT(Ped);
					PED::RESET_PED_VISIBLE_DAMAGE(Ped);
				}
				STOP
				g_Notifications->Push("Clear Visual Damage", "We've cleaned your player ped.");
			});
		});


		g_UiManager->AddSubmenu<RegularSubmenu>("Ped Flags", SubmenuPedFlags, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Swim Anywhere", nullptr, &g_PlayerOptions->m_SwimAnywhere, []
			{
				QUEUE()
				{
					if (!g_PlayerOptions->m_SwimAnywhere) {
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 65, FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Ped Can Fly Trough Windshield", nullptr, &g_PlayerOptions->m_PedCanFlyTroughWindshield, []
			{
				QUEUE()
				{
					if (g_PlayerOptions->m_PedCanFlyTroughWindshield) {
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 32, TRUE);
					}
					else {
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 32, FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Ped Dies By Ragdoll", nullptr, &g_PlayerOptions->m_PedDiesByRagdoll, []
			{
				QUEUE()
				{
					if (g_PlayerOptions->m_PedDiesByRagdoll) {
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 33, TRUE);
					}
					else {
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 33, FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("No Collision", nullptr, &g_PlayerOptions->m_NoCollision, []
			{
				QUEUE()
				{
					if (g_PlayerOptions->m_NoCollision) {
						ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), FALSE, TRUE);
					}
					else {
						ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), TRUE, TRUE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Reduced Collision", nullptr, &g_PlayerOptions->m_ReducedCollision);
			sub->AddOption<BoolOption<bool>>("No Jumping", nullptr, &g_PlayerOptions->m_NoJumping);
			sub->AddOption<BoolOption<bool>>("Shrink", nullptr, &g_PlayerOptions->m_Shrink, []
			{
				QUEUE()
				{
					if (g_PlayerOptions->m_Shrink) {
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, TRUE);
					}
					else {
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Freeze", nullptr, &g_PlayerOptions->m_Freeze, []
			{
				QUEUE()
				{
					if (!g_PlayerOptions->m_Freeze) {
						ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), FALSE);
					}
				}
				STOP
			});
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Ped Proofs", SubmenuPedProofs, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Bullet", nullptr, &g_PlayerOptions->m_BulletProof);
			sub->AddOption<BoolOption<bool>>("Fire", nullptr, &g_PlayerOptions->m_FireProof);
			sub->AddOption<BoolOption<bool>>("Explosion", nullptr, &g_PlayerOptions->m_ExplosionProof);
			sub->AddOption<BoolOption<bool>>("Collision", nullptr, &g_PlayerOptions->m_CollisionProof);
			sub->AddOption<BoolOption<bool>>("Melee", nullptr, &g_PlayerOptions->m_MeleeProof);
			sub->AddOption<BoolOption<bool>>("Drown", nullptr, &g_PlayerOptions->m_DrownProof);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Movement", SubmenuMovement, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberBoolOption<float, bool>>("No Clip", nullptr, &g_PlayerOptions->m_NoClip, &g_PlayerOptions->m_NoClipSpeed, 1.f, 10.f, 0.1f, 1);
			sub->AddOption<BoolOption<bool>>("Super Man", nullptr, &g_PlayerOptions->m_SuperMan);
			sub->AddOption<BoolOption<bool>>("Super Jump", nullptr, &g_PlayerOptions->m_SuperJump);
			sub->AddOption<BoolOption<bool>>("Beast Jump", nullptr, &g_PlayerOptions->m_BeastJump);
			sub->AddOption<BoolOption<bool>>("Graceful Landing", nullptr, &g_PlayerOptions->m_GracefulLanding);
			sub->AddOption<NumberBoolOption<float, bool>>("Slide Run", nullptr, &g_PlayerOptions->m_EnableSlideRun, &g_PlayerOptions->m_SlideRun, 1.f, 10.f, 0.1f, 1);
			sub->AddOption<NumberBoolOption<float, bool>>("Run Multiplier", nullptr, &g_PlayerOptions->m_EnableRunSpeed, &g_PlayerOptions->m_RunSpeed, 1.f, 10.f, 0.1f, 1);
			sub->AddOption<NumberBoolOption<float, bool>>("Swim Multiplier", nullptr, &g_PlayerOptions->m_EnablSwimSpeed, &g_PlayerOptions->m_SwimSpeed, 1.f, 10.f, 0.1f, 1);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Handtrails", SubmenuHandtrails, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Toggle", nullptr, &g_PlayerOptions->m_Handtrails);
			sub->AddOption<BoolOption<bool>>("Rainbow", nullptr, &g_PlayerOptions->m_RainbowHandtrails);
			sub->AddOption<NumberOption<float>>("Scale", nullptr, &g_PlayerOptions->m_HandTrailsSize, 0.1f, 2.f, 0.1f, 1);
			sub->AddOption<NumberOption<int>>("R", nullptr, &g_PlayerOptions->m_Red, 0, 255);
			sub->AddOption<NumberOption<int>>("G", nullptr, &g_PlayerOptions->m_Green, 0, 255);
			sub->AddOption<NumberOption<int>>("B", nullptr, &g_PlayerOptions->m_Blue, 0, 255);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Weapon", SubmenuWeapon, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Gun Locker", nullptr, SubmenuGunLocker);
			sub->AddOption<SubOption>("Modifiers", nullptr, SubmenuModifiers);
			sub->AddOption<SubOption>("Paint Gun", nullptr, SubmenuPaintGun);
			sub->AddOption<SubOption>("Impact", nullptr, SubmenuImpact);
			sub->AddOption<BoolOption<bool>>("Gravity Gun", nullptr, &g_WeaponOptions->m_GravityGun);
			sub->AddOption<BoolOption<bool>>("Airstrike Gun", nullptr, &g_WeaponOptions->m_AirstrikeGun);
			sub->AddOption<BoolOption<bool>>("Teleport Gun", nullptr, &g_WeaponOptions->m_TeleportGun);
			sub->AddOption<BoolOption<bool>>("Fly Gun", nullptr, &g_WeaponOptions->m_FlyGun);
			sub->AddOption<BoolOption<bool>>("Dead Eyes", nullptr, &g_WeaponOptions->m_DeadEyes, []
			{
				QUEUE()
				{
					if (!g_WeaponOptions->m_DeadEyes) {
						MISC::SET_TIME_SCALE(1.f);
						GRAPHICS::ANIMPOSTFX_STOP("HeistLocate");
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Rapid Fire", nullptr, &g_WeaponOptions->m_RapidFire);
			sub->AddOption<BoolOption<bool>>("Clown Gun", nullptr, &g_WeaponOptions->m_ClownGun);
			sub->AddOption<BoolOption<bool>>("Acceleration Gun", "Affected entities: Vehicles", &g_WeaponOptions->m_AccelerationGun);
			sub->AddOption<BoolOption<bool>>("Recolor Gun", "Affected entities: Vehicles", &g_WeaponOptions->m_RecolorGun);
			sub->AddOption<BoolOption<bool>>("Steal Gun", "Affected entities: Vehicles", &g_WeaponOptions->m_StealGun);
			sub->AddOption<BoolOption<bool>>("Delete Gun", "Affected entities: Vehicles, Peds, Objects", &g_WeaponOptions->m_DeleteGun);
			sub->AddOption<BoolOption<bool>>("Ped Gun", nullptr, &g_WeaponOptions->m_PedGun);
			sub->AddOption<BoolOption<bool>>("Flamethrower", nullptr, &g_WeaponOptions->m_FlameThrower);
			sub->AddOption<BoolOption<bool>>("Valkyrie Gun", nullptr, &g_WeaponOptions->m_ValkyrieGun);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Gun Locker", SubmenuGunLocker, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Weapons", nullptr, []
			{
				QUEUE()
				{
					unsigned long int MeleeWeapons[] = { 0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08 };
					unsigned long int Handguns[] = { 0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1, 0x2B5EF5EC, 0x917F6C8C, 0x57A4368C };
					unsigned long int SubMachineGuns[] = { 0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155 };
					unsigned long int Shotguns[] = { 0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D, 0x5A96BA4 };
					unsigned long int AssaultRifles[] = { 0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830, 0x9D1F17E6, 0xC78D71B4, 0xD1D5F52B };
					unsigned long int LightMachineGuns[] = { 0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683 };
					unsigned long int SniperRifles[] = { 0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0, 0x6E7DDDEC };
					unsigned long int HeavyWeapons[] = { 0xB1CA77B1, 0xA284510B, 0x4DD2DC56, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67, 0xDB26713A };
					unsigned long int Throwables[] = { 0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3 };
					unsigned long int MiscellaneousWeapons[] = { 0x34A67B97, 0xFBAB5776, 0x060EC506, 0xBA536372, 0x184140A1 };

					for (int i = 0; i < (sizeof(MeleeWeapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), MeleeWeapons[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(Handguns) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Handguns[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(SubMachineGuns) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), SubMachineGuns[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(Shotguns) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Shotguns[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(AssaultRifles) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), AssaultRifles[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(LightMachineGuns) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), LightMachineGuns[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(SniperRifles) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), SniperRifles[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(HeavyWeapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), HeavyWeapons[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(Throwables) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Throwables[i], 9999, FALSE);
					for (int i = 0; i < (sizeof(MiscellaneousWeapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), MiscellaneousWeapons[i], 9999, FALSE);
				}
				STOP
				g_Notifications->Push("Give all Weapons", "We've added all weapons to your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Weapons", nullptr, []
			{
				QUEUE() { WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), TRUE); } STOP
				g_Notifications->Push("Take all Weapons", "We've removed all weapons from your gun locker.");
			});
			sub->AddOption<SubOption>("Melee Weapons", nullptr, SubmenuMeleeWeapons);
			sub->AddOption<SubOption>("Handguns", nullptr, SubmenuHandguns);
			sub->AddOption<SubOption>("Submachine Guns", nullptr, SubmenuSMGs);
			sub->AddOption<SubOption>("Shotguns", nullptr, SubmenuShotguns);
			sub->AddOption<SubOption>("Assault Rifles", nullptr, SubmenuAssaultRifles);
			sub->AddOption<SubOption>("Light Machine Guns", nullptr, SubmenuLightMachineGuns);
			sub->AddOption<SubOption>("Sniper Rifles", nullptr, SubmenuSniperRifles);
			sub->AddOption<SubOption>("Heavy Weapons", nullptr, SubmenuHeavyWeapons);
			sub->AddOption<SubOption>("Throwables", nullptr, SubmenuThrowables);
			sub->AddOption<SubOption>("Miscellaneous Weapons", nullptr, SubmenuMiscellaneousWeapons);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Melee Weapons", SubmenuMeleeWeapons, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Melee Weapons", nullptr, []
			{
				QUEUE() 
				{
					unsigned long int Weapons[] = { 0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 1, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Melee Weapons", "We've put all Melee Weapons in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Melee Weapons", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Melee Weapons", "We've removed all Melee Weapons from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Antique Cavalry Dagger", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x92A27487, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Baseball Bat", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x958A4A8F, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Broken Bottle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xF9E6AA4B, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Crowbar", "Your sister fucks this.", [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x84BD7BFD, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Flashlight", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x8BB05FD7, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Golf Club", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x440E4788, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Hammer", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x4E875F73, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Hatchet", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xF9DCBF2D, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Brass Knuckles", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xD8DF3C3C, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Knife", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99B507EA, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Machete", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDD5DF8D9, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Switchblade", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDFE37640, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Nightstick", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x678B81B1, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Pipe Wrench", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x19044EE0, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Battle Axe", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xCD274149, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Pool Cue", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x94117305, 1, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Stone Hatchet", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3813FC08, 1, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Handguns", SubmenuHandguns, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Handguns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39,0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1, 0x2B5EF5EC, 0x917F6C8C, 0x57A4368C };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Handguns", "We've put all Handguns in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Handguns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1, 0x2B5EF5EC, 0x917F6C8C, 0x57A4368C };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Handguns", "We've removed all Handguns from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1B06D571, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Pistol Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFE256D4, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Combat Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5EF9FEC4, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give AP Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x22D8FE39, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Stun Gun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3656C8C1, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Pistol .50", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x99AEEB3B, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give SNS Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFD21232, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give SNS Pistol Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x88374054, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Heavy Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xD205520E, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Vintage Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83839C4, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Flare Gun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x47757124, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Marksman Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDC4DB296, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Heavy Revolver", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC1B3C3D1, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Heavy Revolver Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xCB96392F, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Double Action Revolver", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x97EA20B8, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Up-n-Atomizer", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAF3696A1, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Ceramic Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2B5EF5EC, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Navy Revolver", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x917F6C8C, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Perico Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x57A4368C, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Submachine Guns", SubmenuSMGs, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Submachine Guns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Submachine Guns", "We've put all Submachine Guns in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Submachine Guns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Submachine Guns", "We've removed all Submachine Guns from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Micro SMG", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x13532244, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give SMG", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2BE6766B, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give SMG Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x78A97CD0, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Assault SMG", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEFE7E2DF, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Combat PDW", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x0A3D4D34, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Machine Pistol", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDB1AA450, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Mini SMG", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBD248B55, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Unholy Hellbringer", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x476BF155, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Shotguns", SubmenuShotguns, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Shotguns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D, 0x5A96BA4 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Shotguns", "We've put all Shotguns in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Shotguns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D, 0x5A96BA4 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Shotguns", "We've removed all Shotguns from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Pump Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x1D073A89, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Pump Shotgun Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x555AF99A, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Sawed-Off Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7846A318, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Assault Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xE284C527, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Bullpup Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D61E50F, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Musket", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA89CB99E, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Heavy Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x3AABBBAA, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Double Barrel Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEF951FBB, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Sweeper Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x12E82D3D, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Combat Shotgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x5A96BA4, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Assault Rifles", SubmenuAssaultRifles, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Assault Rifles", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830, 0x9D1F17E6, 0xC78D71B4, 0xD1D5F52B };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Assault Rifles", "We've put all Assault Rifles in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Assault Rifles", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830, 0x9D1F17E6, 0xC78D71B4, 0xD1D5F52B };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Assault Rifles", "We've removed all Assault Rifles from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Assault Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBFEFFF6D, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Assault Rifle Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x394F415C, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Carbine Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x83BF0278, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Carbine Rifle Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFAD1F1C9, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Advanced Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAF113F99, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Special Carbine", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC0A3098D, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Special Carbine Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x969C3D67, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Bullpup Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F229F94, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Bullpup Rifle Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x84D6FAFD, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Compact Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x624FE830, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Military Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D1F17E6, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Heavy Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC78D71B4, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Tactical Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xD1D5F52B, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Light Machine Guns", SubmenuLightMachineGuns, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Light Machine Guns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Light Machine Guns", "We've put all Light Machine Guns in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Light Machine Guns", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Light Machine Guns", "We've removed all Light Machine Guns from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give MG", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D07F764, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Combat MG", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7FD62962, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Combat MG Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDBBD7280, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Gusenberg Sweeper", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x61012683, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Sniper Rifles", SubmenuSniperRifles, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Sniper Rifles", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0, 0x6E7DDDEC };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Sniper Rifles", "We've put all Sniper Rifles in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Sniper Rifles", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0, 0x6E7DDDEC };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Sniper Rifles", "We've removed all Sniper Rifles from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Sniper Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x05FC3C11, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Heavy Sniper", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x0C472FE2, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Heavy Sniper Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA914799, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Marksman Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC734385A, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Marksman Rifle Mk II", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6A6C02E0, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Precision Rifle", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6E7DDDEC, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Heavy Weapons", SubmenuHeavyWeapons, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Heavy Weapons", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0xB1CA77B1, 0xA284510B, 0x4DD2DC56, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67, 0xDB26713A };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Heavy Weapons", "We've put all Heavy Weapons in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Heavy Weapons", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0xB1CA77B1, 0xA284510B, 0x4DD2DC56, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67, 0xDB26713A };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Heavy Weapons", "We've removed all Heavy Weapons from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give RPG", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xB1CA77B1, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Grenade Launcher", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA284510B, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Smoke Grenade Launcher", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x4DD2DC56, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Minigun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x42BF8A85, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Firework Launcher", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F7497E5, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Railgun", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6D544C99, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Homing Launcher", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x63AB0442, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Compact Grenade Launcher", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x0781FE4A, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Widowmaker", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xB62D1F67, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Compact EMP Launcher", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xDB26713A, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Throwables", SubmenuThrowables, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Throwables", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					} 
				}
				STOP
				g_Notifications->Push("Give all Throwables", "We've put all Throwables in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Throwables", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Throwables", "We've removed all Throwables from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Grenades", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x93E220BD, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give BZ Gas", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA0973D5E, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Molotov Cocktails", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x24B17070, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Sticky Bombs", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2C3731D9, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Proximity Mines", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAB564B93, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Snowballs", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x787F0BB, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Pipe Bombs", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBA45E8B8, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Baseballs", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x23C9F95C, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Tear Gas", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFDBC8A50, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Flares", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x497FACC3, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Miscellaneous Weapons", SubmenuMiscellaneousWeapons, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give all Miscellaneous Weapons", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x34A67B97, 0xFBAB5776, 0x060EC506, 0xBA536372, 0x184140A1 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, FALSE);
					}
				}
				STOP
				g_Notifications->Push("Give all Miscellaneous Weapons", "We've put all Miscellaneous Weapons in your gun locker.");
			});
			sub->AddOption<RegularOption>("Take all Miscellaneous Weapons", nullptr, []
			{
				QUEUE()
				{
					unsigned long int Weapons[] = { 0x34A67B97, 0xFBAB5776, 0x060EC506, 0xBA536372, 0x184140A1 };
					for (int i = 0; i < (sizeof(Weapons) / 4); i++)
					{
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Weapons[i]);
					}
				}
				STOP
				g_Notifications->Push("Take all Miscellaneous Weapons", "We've removed all Miscellaneous Weapons from your gun locker.");
			});
			sub->AddOption<RegularOption>("Give Jerry Can", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x34A67B97, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Parachute", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Fire Extinguisher", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x060EC506, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Hazardous Jerry Can", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBA536372, 9999, FALSE); } STOP });
			sub->AddOption<RegularOption>("Give Fertilizer Can", nullptr, [] { QUEUE() { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x184140A1, 9999, FALSE); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Modifiers", SubmenuModifiers, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Infinite Ammo", nullptr, &g_WeaponOptions->m_InfiniteAmmo, []
			{
				QUEUE()
				{
					if (!g_WeaponOptions->m_InfiniteAmmo) {
						WEAPON::SET_PED_INFINITE_AMMO(PLAYER::PLAYER_PED_ID(), FALSE, NULL);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Infinite Clip", nullptr, &g_WeaponOptions->m_InfiniteClip, []
			{
				QUEUE()
				{
					if (!g_WeaponOptions->m_InfiniteClip) {
						WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Shotgun Mode", nullptr, &g_WeaponOptions->m_ShotGunMode, []
			{
				if (!g_WeaponOptions->m_ShotGunMode) {
					auto WeaponInfo = Game::GetLocalPed()->m_weapon_manager->m_weapon_info;
					WeaponInfo->m_batch_spread = 0.1f;;
					WeaponInfo->m_bullets_in_batch = 1;
					WeaponInfo->m_force = 200.f;
					WeaponInfo->m_speed = 5000.f;
				}
			});
			sub->AddOption<BoolOption<bool>>("No Recoil", nullptr, &g_WeaponOptions->m_NoRecoil, []
			{
				if (!g_WeaponOptions->m_NoRecoil) {
					if (!Game::GetLocalPed())
						return;

					auto* const WeaponManager = Game::GetLocalPed()->m_weapon_manager;
					if (WeaponManager)
					{
						auto const CurrentWeapon = WeaponManager->m_selected_weapon_hash;
						WeaponManager->m_weapon_info->m_explosion_shake_amplitude = g_WeaponOptions->GetRecoilValue(CurrentWeapon);
					}
				}
			});
			sub->AddOption<BoolOption<bool>>("No Spread", nullptr, &g_WeaponOptions->m_NoSpread, []
			{
				if (!g_WeaponOptions->m_NoSpread) {
					if (!Game::GetLocalPed())
						return;

					auto* const WeaponManager = Game::GetLocalPed()->m_weapon_manager;
					if (WeaponManager)
					{
						auto const CurrentWeapon = WeaponManager->m_selected_weapon_hash;
						WeaponManager->m_weapon_info->m_accuracy_spread = g_WeaponOptions->GetSpreadValue(CurrentWeapon);
					}
				}
			});
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Paint Gun", SubmenuPaintGun, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Toggle", nullptr, &g_WeaponOptions->m_PaintGun);
			sub->AddOption<BoolOption<bool>>("Rainbow", nullptr, &g_WeaponOptions->m_RainbowPaintGun);
			sub->AddOption<NumberOption<float>>("Size", nullptr, &g_WeaponOptions->m_PaintGunSize, 0.1f, 10.f, 0.1f, 1);
			sub->AddOption<NumberOption<float>>("Intensity", nullptr, &g_WeaponOptions->m_Intensity, 1000.f, 10000.f, 1000.f, 1);
			sub->AddOption<RegularOption>("Wash Paint", nullptr, [] { g_WeaponOptions->m_PaintGunCoords.clear(); });
			sub->AddOption<NumberOption<int>>("R", nullptr, &g_WeaponOptions->m_PaintColorR, 0, 255);
			sub->AddOption<NumberOption<int>>("G", nullptr, &g_WeaponOptions->m_PaintColorG, 0, 255);
			sub->AddOption<NumberOption<int>>("B", nullptr, &g_WeaponOptions->m_PaintColorB, 0, 255);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Impact", SubmenuImpact, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Toggle", nullptr, &g_WeaponOptions->m_ImpactGun);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Type", nullptr, &g_WeaponOptions->m_ExplosionTag, &g_WeaponOptions->m_ExplosionTagPos);
			sub->AddOption<NumberOption<float>>("Damage Scale", nullptr, &g_WeaponOptions->m_DamageScale, 0.f, 2000.f, 250.f, 1);
			sub->AddOption<BoolOption<bool>>("Audible", nullptr, &g_WeaponOptions->m_Audible);
			sub->AddOption<BoolOption<bool>>("Visible", nullptr, &g_WeaponOptions->m_Visible);
			sub->AddOption<NumberOption<float>>("Camera Shake", nullptr, &g_WeaponOptions->m_CameraShake, 0.f, 1000.f, 250.f, 1);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle", SubmenuVehicle, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Handling", nullptr, SubmenuHandling);
			sub->AddOption<BoolOption<bool>>("God", nullptr, &g_VehicleOptions->m_God, []
			{
				QUEUE()
				{
					if (!g_VehicleOptions->m_God) {
						auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehicle, TRUE);
						ENTITY::SET_ENTITY_INVINCIBLE(Vehicle, FALSE);
						ENTITY::SET_ENTITY_CAN_BE_DAMAGED(Vehicle, TRUE);
						ENTITY::SET_ENTITY_PROOFS(Vehicle, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE);
						VEHICLE::SET_VEHICLE_STRONG(Vehicle, FALSE);
						VEHICLE::SET_VEHICLE_BODY_HEALTH(Vehicle, 1000);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Invisibility", nullptr, &g_VehicleOptions->m_Invisibility, []
			{
				QUEUE()
				{
					if (!g_VehicleOptions->m_Invisibility) {
						ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE, FALSE);
						NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Hand Brake", nullptr, &g_VehicleOptions->m_HandBrake);
			sub->AddOption<BoolOption<bool>>("Bypass Max Speed", nullptr, &g_VehicleOptions->m_BypassMaxSpeed, []
			{
				QUEUE()
				{
					if (!g_VehicleOptions->m_BypassMaxSpeed) {
						VEHICLE::SET_VEHICLE_MAX_SPEED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 0.f);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Scorched Render", nullptr, &g_VehicleOptions->m_ScorchedRender, []
			{
				QUEUE()
				{
					if (!g_VehicleOptions->m_ScorchedRender) {
						ENTITY::SET_ENTITY_RENDER_SCORCHED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Drive On Water", nullptr, &g_VehicleOptions->m_DriveOnWater);
			sub->AddOption<BoolOption<bool>>("Drive Under Water", nullptr, &g_VehicleOptions->m_DriveUnderWater);
			sub->AddOption<NumberBoolOption<float, bool>>("Vehicle Fly", nullptr, &g_VehicleOptions->m_VehicleFly, &g_VehicleOptions->m_VehicleFlySpeed, 1.f, 10.f, 0.1f, 1);
			sub->AddOption<BoolOption<bool>>("Rainbow Paint", nullptr, &g_VehicleOptions->m_RainbowColor);
			sub->AddOption<BoolOption<bool>>("Rainbow Neons", nullptr, &g_VehicleOptions->m_RainbowNeons);
			sub->AddOption<BoolOption<bool>>("Rainbow Tire Smoke", nullptr, &g_VehicleOptions->m_RainbowTireSmoke);
			sub->AddOption<BoolOption<bool>>("Allow Heavy Weapons", "You can use stuff like RPG in vehicle", &g_VehicleOptions->m_AllowHeavyWeapons);
			sub->AddOption<RegularOption>("Stop", nullptr, [] { QUEUE() { VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 0.f); } STOP });
			sub->AddOption<RegularOption>("Forge Model", "Won't save in garage", [] {
				if (Game::GetLocalPed()->m_vehicle->m_model_info->m_hash) {
					Game::GetLocalPed()->m_vehicle->m_model_info->m_hash = 0xE8A8BDA8;
				}
			});
			sub->AddOption<RegularOption>("Repair", nullptr, []
			{
				QUEUE()
				{
					auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
					g_Functions->ChangeNetworkObjectOwner(Vehicle, g_Pointers->m_GetNetPlayer(PLAYER::PLAYER_ID()));
					VEHICLE::SET_VEHICLE_FIXED(Vehicle);
					VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
					VEHICLE::SET_VEHICLE_UNDRIVEABLE(Vehicle, FALSE);
					VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0.f);
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle, "Sentinel");
				}
				STOP
			});
			sub->AddOption<RegularOption>("Upgrade", nullptr, []
			{
				QUEUE()
				{
					auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
					g_Functions->ChangeNetworkObjectOwner(Vehicle, g_Pointers->m_GetNetPlayer(PLAYER::PLAYER_ID()));
					VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
					for (int i = 0; i < 50; i++) {
						VEHICLE::SET_VEHICLE_MOD(Vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, i) - 1, false);
					}
					VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, 20, TRUE);
					VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle, 245, 66, 66);
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle, "Sentinel");
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle, 245, 66, 66);
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle, 0, 0, 0);
					VEHICLE::SET_VEHICLE_FIXED(Vehicle);
					VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
					VEHICLE::SET_VEHICLE_UNDRIVEABLE(Vehicle, FALSE);
					VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Vehicle, FALSE);
					VEHICLE::SET_VEHICLE_WINDOW_TINT(Vehicle, 1);
					VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, 22, TRUE);
					VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(Vehicle, 9);
					if (!VEHICLE::GET_VEHICLE_NEON_ENABLED(Vehicle, 0) || !VEHICLE::GET_VEHICLE_NEON_ENABLED(Vehicle, 1) || !VEHICLE::GET_VEHICLE_NEON_ENABLED(Vehicle, 2) || !VEHICLE::GET_VEHICLE_NEON_ENABLED(Vehicle, 3))
					{
						VEHICLE::SET_VEHICLE_NEON_ENABLED(Vehicle, 0, TRUE);
						VEHICLE::SET_VEHICLE_NEON_ENABLED(Vehicle, 1, TRUE);
						VEHICLE::SET_VEHICLE_NEON_ENABLED(Vehicle, 2, TRUE);
						VEHICLE::SET_VEHICLE_NEON_ENABLED(Vehicle, 3, TRUE);
					}
					VEHICLE::SET_VEHICLE_NEON_COLOUR(Vehicle, 245, 66, 66);
					VEHICLE::SET_VEHICLE_FIXED(Vehicle);
					VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
					VEHICLE::SET_VEHICLE_UNDRIVEABLE(Vehicle, FALSE);
					VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0.f);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Add Marker", nullptr, []
			{
				QUEUE()
				{
					auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);

					if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), NULL))
						return;

					g_Functions->ChangeNetworkObjectOwner(Vehicle, g_Pointers->m_GetNetPlayer(PLAYER::PLAYER_ID()));

					int Blip;
					Blip = HUD::ADD_BLIP_FOR_ENTITY(Vehicle);
					HUD::SET_BLIP_SPRITE(Blip, 225);
					HUD::SET_BLIP_NAME_FROM_TEXT_FILE(Blip, "Sentinel");
				}
				STOP
			});
			sub->AddOption<KeyboardOption>("Custom Plate Text", nullptr, [] { QUEUE() { VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), g_UiManager->m_KeyboardInput); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Handling", SubmenuHandling, [](RegularSubmenu* sub)
		{
			if (Game::GetLocalPed() == nullptr) 
				return;
			if (Game::GetLocalPed()->m_vehicle == nullptr) 
				return;
			if (Game::GetLocalPed()->m_vehicle->m_handling_data == nullptr) 
				return;
			auto Data = Game::GetLocalPed()->m_vehicle->m_handling_data;

			sub->AddOption<NumberOption<float>>("Mass", nullptr, &Data->m_mass, 0.f, 50000.f, 10.f, 2);
			sub->AddOption<NumberOption<float>>("Accerlation", nullptr, &Data->m_acceleration, 0.1f, 100.f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Brake Force", nullptr, &Data->m_brake_force, 0.1f, 100.f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Deformation Multiplier", nullptr, &Data->m_deformation_mult, 0.1f, 100.f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Minimum Traction", nullptr, &Data->m_traction_curve_min, 0.f, 7.f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Maximum Traction", nullptr, &Data->m_traction_curve_max, 0.f, 7.f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Traction Curve", nullptr, &Data->m_traction_curve_lateral, 0.f, 7.f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Burnout Multiplier", nullptr, &Data->m_low_speed_traction_loss_mult, 0.f, 10.f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Suspension Force", nullptr, &Data->m_low_speed_traction_loss_mult, 0.f, 5.0f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Suspension Upper Limit", nullptr, &Data->m_suspension_upper_limit, -1.f, 1.0f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Suspension Lower Limit", nullptr, &Data->m_suspension_lower_limit, -1.f, 1.0f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Suspension Height", nullptr, &Data->m_suspension_raise, -1.f, 1.0f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Steering Lock", nullptr, &Data->m_steering_lock, -90.f, 90.0f, 1.0f, 2);

			sub->AddOption<NumberOption<float>>("Roll Centre Height Front", nullptr, &Data->m_roll_centre_height_front, -1.f, 1.0f, 0.1f, 2);
			sub->AddOption<NumberOption<float>>("Roll Centre Height Back", nullptr, &Data->m_roll_centre_height_rear, -1.f, 1.0f, 0.1f, 2);

			sub->AddOption<NumberOption<float>>("Anti Roll Bar Force", nullptr, &Data->m_anti_rollbar_force, 0.f, 10.f, 1.00f, 2);
			sub->AddOption<NumberOption<float>>("Buoyancy", nullptr, &Data->m_buoyancy, .01f, 99, 0.01f, 2);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Spawner", SubmenuSpawner, [](RegularSubmenu* sub)
		{
			g_Functions->m_VehicleSpawnerOpen = true;
			sub->AddOption<SubOption>("Spawn Settings", nullptr, SubmenuSpawnSettings);
			sub->AddOption<SubOption>("Compacts", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 0; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Sedans", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 1; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("SUVs", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 2; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Coupes", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 3; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Muscle", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 4; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Sports Classics", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 5; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Sports", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 6; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Super", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 7; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Motorcycles", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 8; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Off-Road", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 9; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Industrial", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 10; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Utility", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 11; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Vans", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 12; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Cycles", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 13; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Boats", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 14; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Helicopters", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 15; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Planes", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 16; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Service", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 17; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Emergency", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 18; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Military", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 19; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Commercial", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 20; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Trains", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 21; g_Functions->UpdateVehicleSpawner(); });
			sub->AddOption<SubOption>("Open Wheel", nullptr, SubmenuVehicleSpawner, [] { g_Functions->m_VehicleSpawnerVector.clear(); g_Functions->m_SelectedClass = 22; g_Functions->UpdateVehicleSpawner(); });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Spawn Settings", SubmenuSpawnSettings, [](RegularSubmenu* sub)
		{
			g_Functions->m_VehicleSpawnerOpen = true;
			sub->AddOption<BoolOption<bool>>("Spawn Upgraded", nullptr, &g_Functions->m_SpawnUpgraded);
			sub->AddOption<BoolOption<bool>>("Spawn In Vehicle", nullptr, &g_Functions->m_SpawnInVehicle);
			sub->AddOption<BoolOption<bool>>("Delete Previous", nullptr, &g_Functions->m_DeletePrevious);
			sub->AddOption<BoolOption<bool>>("Despawn Bypass", nullptr, &g_Functions->m_SinglePlayerBypass);
			sub->AddOption<BoolOption<bool>>("Prevent Crash", nullptr, &g_Functions->m_PreventCrash);
			sub->AddOption<BoolOption<bool>>("Spawn Air Vehicles In Air", nullptr, &g_Functions->m_SpawnInAir);
			sub->AddOption<BoolOption<bool>>("Fade Animation", nullptr, &g_Functions->m_Fade);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Spawner", SubmenuVehicleSpawner, [](RegularSubmenu* sub)
		{
			g_Functions->m_VehicleSpawnerOpen = true;

			for (int Index = 0; Index < g_Functions->m_VehicleSpawnerVector.size(); Index++)
			{
				sub->AddOption<RegularOption>(g_Functions->m_VehicleSpawnerVector[Index].m_Name.c_str(), nullptr, [=]
				{
					g_Functions->SpawnVehicle(g_Functions->m_VehicleSpawnerVector[Index].m_Model, g_Functions->m_SpawnUpgraded, g_Functions->m_SpawnInVehicle, g_Functions->m_DeletePrevious, g_Functions->m_SinglePlayerBypass, g_Functions->m_PreventCrash, g_Functions->m_SpawnInAir, g_Functions->m_Fade);
				});
			}
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Network", SubmenuNetwork, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Session Management", nullptr, SubmenuSessionManagement);
			sub->AddOption<SubOption>("Protections", nullptr, SubmenuProtections);
			sub->AddOption<SubOption>("Playerlist", nullptr, SubmenuPlayerList);
			sub->AddOption<SubOption>("Session Starter", nullptr, SubmenuSessionStarter);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Session Management", SubmenuSessionManagement, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("ESP", nullptr, SubmenuESP);
			sub->AddOption<BoolOption<bool>>("Sync Fuzzer", "Crashes everyone near you.", &g_NetworkOptions->m_SyncFuzzer);
			sub->AddOption<BreakOption>("Host Management");
			sub->AddOption<RegularOption>("Force Script Host", nullptr, [] 
			{ 
				QUEUE() 
				{ 
					if (!*g_Pointers->m_OnlineMode)
					{
						g_Notifications->PushError("Force Script Host", "You can't use this in offline.");
						return;
					}

					if (!g_Functions->IsScriptHost(PLAYER::PLAYER_ID())) 
					{
						if (Game::ForceHost(RAGE_JOAAT("freemode")))
						{
							g_Notifications->Push("Force Script Host", "You're now the Script Host.");
						}
						else
						{
							g_Notifications->PushError("Force Script Host", "Failed, couldn't force Script Host.");
							return;
						}
					}
				} 
				STOP 
			});
			sub->AddOption<BoolOption<bool>>("Always Script Host", nullptr, &g_NetworkOptions->m_AlwaysScriptHost);
			sub->AddOption<BreakOption>("Event Management");
			sub->AddOption<RegularOption>("Send To Island", nullptr, []
			{
				QUEUE()
				{
					for (std::uint32_t Index = 0; Index < 32; Index++)
					{
						const size_t ArgCount = 2;
						std::int64_t Args[ArgCount] =
						{
							(std::int64_t)eRemoteEvent::SendToCayoPerico,
							(std::int64_t)PLAYER::PLAYER_ID(),
						};

						g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << Index);
					}
				}
				STOP
			});
			sub->AddOption<RegularOption>("Vehicle Kick", nullptr, []
			{
				QUEUE()
				{
					for (std::uint32_t Index = 0; Index < 32; Index++)
					{
						const size_t ArgCount = 2;
						std::int64_t Args[ArgCount] =
						{
							(std::int64_t)eRemoteEvent::VehicleKick,
							(std::int64_t)PLAYER::PLAYER_ID(),
						};

						g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << Index);
					}
				}
				STOP
			});
			sub->AddOption<RegularOption>("Sound Spam", nullptr, []
			{
				QUEUE()
				{
					for (std::uint32_t Index = 0; Index < 32; Index++)
					{
						const size_t ArgCount = 2;
						std::int64_t Args[ArgCount] =
						{
							(std::int64_t)eRemoteEvent::SoundSpam,
							(std::int64_t)PLAYER::PLAYER_ID()
						};

						g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << Index);
					}
				}
				STOP
			});
			sub->AddOption<BreakOption>("Vehicle Management");
			sub->AddOption<BoolOption<bool>>("Disable Oppressor Mk1", nullptr, &g_NetworkOptions->m_DisableMk1);
			sub->AddOption<BoolOption<bool>>("Disable Oppressor Mk2", nullptr, &g_NetworkOptions->m_DisableMk2);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("ESP", SubmenuESP, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Line ESP", nullptr, &g_NetworkOptions->m_LineESP);
			sub->AddOption<BoolOption<bool>>("Name ESP", nullptr, &g_NetworkOptions->m_NameESP);
			sub->AddOption<BoolOption<bool>>("Box ESP", nullptr, &g_NetworkOptions->m_BoxESP);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Protections", SubmenuProtections, [](RegularSubmenu* sub)
		{			
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Script Events", nullptr, &g_Protections->m_ScriptEvents, &g_Protections->m_ScriptEventsPos);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Network Events", nullptr, &g_Protections->m_NetworkEvents, &g_Protections->m_NetworkEventsPos);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Stat Events", nullptr, &g_Protections->m_StatEvents, &g_Protections->m_StatEventsPos);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Overworked Renderer", nullptr, &g_Protections->m_Fragments, &g_Protections->m_FragmentsPos);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Invalid Models", nullptr, &g_Protections->m_InvalidModels, &g_Protections->m_InvalidModelsPos);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Next Gen Vehicles", nullptr, &g_Protections->m_NextGenVehicles, &g_Protections->m_NextGenVehiclesPos);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Vote Kick", nullptr, &g_Protections->m_VoteKick, &g_Protections->m_VoteKickPos);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Weapon Crash", nullptr, &g_Protections->m_WeaponCrash, &g_Protections->m_WeaponCrashList);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Task Crash", nullptr, &g_Protections->m_TaskCrash, &g_Protections->m_TaskCrashList);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Playerlist", SubmenuPlayerList, [](RegularSubmenu* sub)
		{
		    if (*g_Pointers->m_OnlineMode)
			{
				// Playerlist using classes, if you use a native in a dx tick the game will crash
				auto PlayerManager = Game::GetNetworkPlayerMgr();
				if (PlayerManager != nullptr)
				{
					for (auto Player : PlayerManager->m_player_list)
					{
						if (Player != nullptr)
						{
							if (Player->is_valid())
							{
								auto Name = g_Pointers->m_GetPlayerName(Player->m_player_id);
								std::string ModifiedString = Name;

								if (Player->is_host())
									ModifiedString.append(" [H]");

								sub->AddOption<SubOption>(ModifiedString.c_str(), nullptr, SubmenuSelectedPlayer, [=]
								{
									g_SelectedPlayer = Player->m_player_id;
									g_SelectedNetPlayer = Player;
									g_PlayerKicked = false;
								});
							}
						}
					}
				}
			}
			else
			{
				// In SP the player id is always 0
				sub->AddOption<SubOption>(g_Pointers->m_GetPlayerName(0), nullptr, SubmenuSelectedPlayer, [=]
				{
					g_SelectedPlayer = 0;
					g_SelectedNetPlayer = g_Pointers->m_GetNetPlayer(0);
					g_PlayerKicked = false;
				});
			}
		});

		g_UiManager->AddSubmenu<PlayerSubmenu>(&g_SelectedPlayer, SubmenuSelectedPlayer, [](PlayerSubmenu* sub)
		{
			sub->AddOption<SubOption>("Removals", nullptr, SubmenuRemovals);
			sub->AddOption<SubOption>("Remote", nullptr, SubmenuRemote);
			sub->AddOption<SubOption>("Social Club", nullptr, SubmenuSocialClub);
			sub->AddOption<SubOption>("Teleport", nullptr, SubmenuNTeleport);
			sub->AddOption<SubOption>("Griefing", nullptr, SubmenuGriefing);
			sub->AddOption<SubOption>("Money Drop", nullptr, SubmenuMoneyDrop);
			sub->AddOption<BoolOption<bool>>("Spectate", nullptr, &g_NetworkOptions->m_Spectate);
			sub->AddOption<RegularOption>("Clone Player", nullptr, [] { PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), TRUE, FALSE, TRUE); });
			sub->AddOption<RegularOption>("Attach", nullptr, []
			{
				QUEUE()
				{
					if (g_SelectedPlayer != PLAYER::PLAYER_ID())
						ENTITY::ATTACH_ENTITY_TO_ENTITY(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0x00, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, TRUE, TRUE, FALSE, TRUE, 2, TRUE, FALSE);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Detach", nullptr, [] { ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), TRUE, TRUE); });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Removals", SubmenuRemovals, [](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Breakup Kick", Removals::m_BreakupKickPos == 1 ? "Displays a cheater message" : "Displays a regular leave message", & Removals::m_BreakupKick, & Removals::m_BreakupKickPos, false, [] { QUEUE() { Removals::BreakupKick(g_SelectedNetPlayer); g_PlayerKicked = true; } STOP });
			sub->AddOption<RegularOption>("Host Kick", nullptr, [] { QUEUE() { Removals::HostKick(g_SelectedNetPlayer); g_PlayerKicked = true; } STOP });
			sub->AddOption<RegularOption>("Desync Kick", nullptr, [] { QUEUE() { Removals::DesyncKick(g_SelectedNetPlayer); g_PlayerKicked = true; } STOP });
			sub->AddOption<RegularOption>("SH Kick", nullptr, [] { QUEUE() { Removals::ScriptHostKick(g_SelectedPlayer); g_PlayerKicked = true; } STOP });
			sub->AddOption<RegularOption>("Fragment Crash", nullptr, [] { QUEUE() { Removals::FragmentCrash(g_SelectedPlayer); g_PlayerKicked = true; } STOP });
			sub->AddOption<RegularOption>("TSE Crash", nullptr, [] { QUEUE() { Removals::ScriptCrash(g_SelectedPlayer); g_PlayerKicked = true; } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Remote", SubmenuRemote, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Send To Island", nullptr, []
			{
				QUEUE()
				{
					const size_t ArgCount = 2;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::SendToCayoPerico,
						(std::int64_t)PLAYER::PLAYER_ID(),
					};

					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			}); 
			sub->AddOption<RegularOption>("Send To Job", nullptr, []
			{
				QUEUE()
				{
					const size_t ArgCount = 2;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::ForceMission,
						(std::int64_t)PLAYER::PLAYER_ID(),
					};
	
					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Vehicle Kick", nullptr, []
			{
				QUEUE()
				{
					const size_t ArgCount = 2;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::VehicleKick,
						(std::int64_t)PLAYER::PLAYER_ID(),
					};

					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Sound Spam", nullptr, []
			{
				QUEUE()
				{
					const size_t ArgCount = 2;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::SoundSpam,
						(std::int64_t)PLAYER::PLAYER_ID()
					};

					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Spectate Notification", nullptr, []
			{
				QUEUE()
				{
					const size_t ArgCount = 3;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::Notification,
						(std::int64_t)PLAYER::PLAYER_ID(),
						(std::int64_t)eRemoteEvent::Spectate
					};

					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			});
			static int g_MoneyStolen{ 500000 };
			sub->AddOption<NumberOption<int>>("Money Stolen", nullptr, &g_MoneyStolen, 0, 15000000, 500000, 3, false, "", "", []
			{
				QUEUE()
				{
					const size_t ArgCount = 4;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::Notification,
						(std::int64_t)PLAYER::PLAYER_ID(),
						(std::int64_t)eRemoteEvent::NotificationMoneyStolen,
						g_MoneyStolen
					};

					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			});			
			static int g_MoneyBanked{ 500000 };
			sub->AddOption<NumberOption<int>>("Money Banked", nullptr, &g_MoneyBanked, 0, 15000000, 500000, 3, false, "", "", []
			{
				QUEUE()
				{
					const size_t ArgCount = 4;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::Notification,
						(std::int64_t)PLAYER::PLAYER_ID(),
						(std::int64_t)eRemoteEvent::NotificationMoneyBanked,
						g_MoneyBanked
					};

					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			});
			static int g_MoneyRemoved{ 500000 };
			sub->AddOption<NumberOption<int>>("Money Removed", nullptr, &g_MoneyRemoved, 0, 15000000, 500000, 3, false, "", "", []
			{
				QUEUE()
				{
					const size_t ArgCount = 4;
					std::int64_t Args[ArgCount] =
					{
						(std::int64_t)eRemoteEvent::Notification,
						(std::int64_t)PLAYER::PLAYER_ID(),
						(std::int64_t)eRemoteEvent::NotificationMoneyRemoved,
						g_MoneyRemoved
					};

					g_Pointers->m_TriggerScriptEvent(1, Args, ArgCount, 1 << g_SelectedPlayer);
				}
				STOP
			});
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Social Club", SubmenuSocialClub, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Copy SCID", nullptr, []
			{
				auto SCID = g_SelectedNetPlayer->get_net_data()->m_gamer_handle.m_rockstar_id;
				auto Name = g_SelectedNetPlayer->get_name();
				auto String = "Player " + std::string(Name) + "'s SCID: " + std::to_string(SCID);
				ImGui::SetClipboardText(String.c_str());
			});
			sub->AddOption<RegularOption>("Show Profile", nullptr, []
			{
				QUEUE()
				{
					Any Handle[76];
					NETWORK::NETWORK_HANDLE_FROM_PLAYER(g_SelectedPlayer, &Handle[0], 13);
					NETWORK::NETWORK_SHOW_PROFILE_UI(&Handle[0]);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Add Friend", nullptr, []
			{
				QUEUE()
				{
					Any Handle[76];
					NETWORK::NETWORK_HANDLE_FROM_PLAYER(g_SelectedPlayer, &Handle[0], 13);
					NETWORK::NETWORK_ADD_FRIEND(&Handle[0], "Can we be friends UwU");
				}
				STOP
			});
			sub->AddOption<RegularOption>("Join Crew", nullptr, []
			{
				QUEUE()
				{
					Any Handle; Any Description;
					NETWORK::NETWORK_CLAN_PLAYER_GET_DESC(&Description, 35, &Handle);
					NETWORK::NETWORK_CLAN_JOIN(Handle);
				}
				STOP
			});
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Teleport", SubmenuNTeleport, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Teleport to Player", nullptr, []
			{
				QUEUE()
				{
					auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), TRUE);
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Teleport in Vehicle", nullptr, []
			{
				QUEUE()
				{
					auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), FALSE);
					for (int index = -1; index < 16; index++)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, index, FALSE))
							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, index);
					}
				}
				STOP
			});
			sub->AddOption<RegularOption>("Teleport Vehicle to Me", nullptr, []
			{
				QUEUE()
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), FALSE))
					{
						auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), FALSE);
						auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
						if (g_Functions->ChangeNetworkObjectOwner(Vehicle, g_Pointers->m_GetNetPlayer(PLAYER::PLAYER_ID())))
							ENTITY::SET_ENTITY_COORDS(Vehicle, Coords.x, Coords.y, Coords.z + 1.0f, FALSE, FALSE, FALSE, FALSE);
					}
				}
				STOP
			});
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Griefing", SubmenuGriefing, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Airstrike", nullptr, []
			{
				QUEUE()
				{
					auto Airstrike = RAGE_JOAAT("WEAPON_AIRSTRIKE_ROCKET");
					auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
					WEAPON::REQUEST_WEAPON_ASSET(Airstrike, 31, FALSE);
					while (!WEAPON::HAS_WEAPON_ASSET_LOADED(Airstrike))
						Script::GetCurrent()->ScriptYield(5ms);
					auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 50, Coords.x, Coords.y, Coords.z, 250, FALSE, PLAYER::PLAYER_PED_ID(), Ped, TRUE, TRUE, 500);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 45, Coords.x, Coords.y, Coords.z, 250, FALSE, PLAYER::PLAYER_PED_ID(), Ped, TRUE, TRUE, 500);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 70, Coords.x, Coords.y, Coords.z, 100, FALSE, PLAYER::PLAYER_PED_ID(), Ped, TRUE, TRUE, 500);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Kill", nullptr, []
			{
				QUEUE()
				{
					auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), TRUE);
					FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, eExplosionTag::EXP_TAG_BLIMP, 1000.f, FALSE, TRUE, 0.f, FALSE);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Silent Kill", nullptr, []
			{				
				QUEUE()
				{
					auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
					auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
					FIRE::ADD_OWNED_EXPLOSION(Ped, Coords.x, Coords.y, Coords.z, eExplosionTag::EXP_TAG_BLIMP, 1000.f, FALSE, TRUE, 0.f);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Taze", nullptr, []
			{
				QUEUE()
				{
					auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);

					constexpr auto MaxAttempts = 20;
					for (std::size_t Attempts = 0; Attempts < MaxAttempts; Attempts++)
					{
						auto Destination = PED::GET_PED_BONE_COORDS(Ped, (int)0x0, 0.0f, 0.0f, 0.0f);
						auto Origin = PED::GET_PED_BONE_COORDS(Ped, (int)0xDEAD, 0.0f, 0.0f, 0.2f);

						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Origin.x, Origin.y, Origin.z, Destination.x, Destination.y, Destination.z, 1, 0, RAGE_JOAAT("WEAPON_STUNGUN"), PLAYER::PLAYER_PED_ID(), false, true, 1);
					}
				}
				STOP
			});
			sub->AddOption<RegularOption>("Cage", nullptr, []
			{
				QUEUE()
				{
					auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), TRUE);
					OBJECT::CREATE_OBJECT(RAGE_JOAAT("prop_gold_cont_01"), Coords.x, Coords.y, Coords.z - 1.f, TRUE, FALSE, FALSE);
				}
				STOP
			});
			sub->AddOption<RegularOption>("Smash", nullptr, []
			{
				QUEUE()
				{
					auto Hash = RAGE_JOAAT("bus");
					while (!STREAMING::HAS_MODEL_LOADED(Hash))
					{
						STREAMING::REQUEST_MODEL(Hash);
						Script::GetCurrent()->ScriptYield();
					}

					NativeVector3 Min, Max;
					MISC::GET_MODEL_DIMENSIONS(Hash, &Min, &Max);
					float Offset = Max.y * 1.6f;
					auto Direction = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer));
					auto Rotation = ENTITY::GET_ENTITY_ROTATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0.f).z;

					auto Position = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0.0f, -10.0f, 0.0f);
					auto Vehicle = VEHICLE::CREATE_VEHICLE(Hash, Position.x + (Direction.x * Offset), Position.y + (Direction.y * Offset), Position.z, Rotation, TRUE, FALSE, FALSE);

					Script::GetCurrent()->ScriptYield();
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
					if (NETWORK::NETWORK_IS_SESSION_STARTED())
					{
						DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
						ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
						auto ID = NETWORK::VEH_TO_NET(Vehicle);
						if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
							NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(ID, true);
						VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
					}

					VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, 0.f);
					ENTITY::SET_ENTITY_VISIBLE(Vehicle, TRUE, NULL);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 700.f);
				}
				STOP
			});
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Money Drop", SubmenuMoneyDrop, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &g_NetworkOptions->m_MoneyDrop);
			sub->AddOption<NumberOption<int>>("Delay", nullptr, &g_NetworkOptions->m_MoneyDropDelay, 0, 2000);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Session Starter", SubmenuSessionStarter, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Join Public", nullptr, [] { g_Functions->StartSession(eSessionType::JOIN_PUBLIC); });
			sub->AddOption<RegularOption>("New Public", nullptr, [] { g_Functions->StartSession(eSessionType::NEW_PUBLIC); });
			sub->AddOption<RegularOption>("Closed Crew", nullptr, [] { g_Functions->StartSession(eSessionType::CLOSED_CREW); });
			sub->AddOption<RegularOption>("Crew", nullptr, [] { g_Functions->StartSession(eSessionType::CREW); });
			sub->AddOption<RegularOption>("Closed Friends", nullptr, [] { g_Functions->StartSession(eSessionType::CLOSED_FRIENDS); });
			sub->AddOption<RegularOption>("Find Friends", nullptr, [] { g_Functions->StartSession(eSessionType::FIND_FRIEND); });
			sub->AddOption<RegularOption>("Solo", nullptr, [] { g_Functions->StartSession(eSessionType::SOLO); });
			sub->AddOption<RegularOption>("Invite Only", nullptr, [] { g_Functions->StartSession(eSessionType::INVITE_ONLY); });
			sub->AddOption<RegularOption>("Join Crew", nullptr, [] { g_Functions->StartSession(eSessionType::JOIN_CREW); });
			sub->AddOption<RegularOption>("Social Club TV", nullptr, [] { g_Functions->StartSession(eSessionType::SC_TV); });
			sub->AddOption<RegularOption>("Leave Online", nullptr, [] { g_Functions->StartSession(eSessionType::LEAVE_ONLINE); });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("World", SubmenuWorld, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Time", nullptr, SubmenuTime);
			sub->AddOption<SubOption>("Weather", nullptr, SubmenuWeather);
			sub->AddOption<SubOption>("Clouds", nullptr, SubmenuClouds);
			sub->AddOption<SubOption>("Glow", nullptr, SubmenuGlow);
			sub->AddOption<BoolOption<bool>>("Bypass Restricted Area", nullptr, &g_WorldOptions->m_BypassRestrictedArea);
			sub->AddOption<RegularOption>("Disable Mission Circles", nullptr, [] { QUEUE() { MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("fmmc_launcher"); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Time", SubmenuTime, [](RegularSubmenu* sub)
		{
			static int g_ClockTime{ 0 };
			sub->AddOption<NumberOption<int>>("Hour", nullptr, &g_ClockTime, 0, 23);
			sub->AddOption<RegularOption>("Set Time", nullptr, [] { QUEUE() { CLOCK::SET_CLOCK_TIME(g_ClockTime, 0, 0); } STOP });
			sub->AddOption<BoolOption<bool>>("Freeze Time", nullptr, &g_WorldOptions->m_FreezeTime);
			sub->AddOption<BoolOption<bool>>("Sync Time", nullptr, &g_WorldOptions->m_SyncTime);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Weather", SubmenuWeather, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Extra Sunny", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("EXTRASUNNY"); } STOP });
			sub->AddOption<RegularOption>("Clear", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("CLEAR"); } STOP });
			sub->AddOption<RegularOption>("Clouds", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("CLOUDS"); } STOP });
			sub->AddOption<RegularOption>("Smog", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("SMOG"); } STOP });
			sub->AddOption<RegularOption>("Foggy", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("FOGGY"); } STOP });
			sub->AddOption<RegularOption>("Overcast", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("OVERCAST"); } STOP });
			sub->AddOption<RegularOption>("Rain", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("RAIN"); } STOP });
			sub->AddOption<RegularOption>("Thunder", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("THUNDER"); } STOP });
			sub->AddOption<RegularOption>("Clearing", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("CLEARING"); } STOP });
			sub->AddOption<RegularOption>("Neutral", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("NEUTRAL"); } STOP });
			sub->AddOption<RegularOption>("Snow", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("SNOW"); } STOP });
			sub->AddOption<RegularOption>("Blizzard", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("BLIZZARD"); } STOP });
			sub->AddOption<RegularOption>("Snowlight", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("SNOWLIGHT"); } STOP });
			sub->AddOption<RegularOption>("Xmas", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("XMAS"); } STOP });
			sub->AddOption<RegularOption>("Halloween", nullptr, [] { QUEUE() { MISC::SET_WEATHER_TYPE_NOW_PERSIST("HALLOWEEN"); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Clouds", SubmenuClouds, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Clear", nullptr, [] { QUEUE() { MISC::UNLOAD_ALL_CLOUD_HATS(); } STOP });
			sub->AddOption<RegularOption>("Cloudy", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Cloudy 01", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Rain", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("RAIN", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Horizon", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Horizon", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Horizon Band 1", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("horizonband1", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Horizon Band 2", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("horizonband2", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Horizon Band 3", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("horizonband3", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Puffs", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Puffs", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Wispy", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Wispy", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Stormy", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Stormy 01", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Clear", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Clear 01", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Snowy", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Snowy 01", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Contrails", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Contrails", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Altostratus", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("altostratus", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Nimbus", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Nimbus", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Cirrus", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Cirrus", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Cirrocumulus", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("cirrocumulus", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Stratocumulus", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("stratocumulus", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Stripey", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("Stripey", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Horsey", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("horsey", 0.5f); } STOP });
			sub->AddOption<RegularOption>("Shower", nullptr, [] { QUEUE() { MISC::LOAD_CLOUD_HAT("shower", 0.5f); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Glow", SubmenuGlow, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Toggle", nullptr, &g_WorldOptions->m_WorldGlow);
			sub->AddOption<BoolOption<bool>>("Rainbow", nullptr, &g_WorldOptions->m_RainbowGlow);
			sub->AddOption<NumberOption<int>>("R", nullptr, &g_WorldOptions->m_Red, 0, 255);
			sub->AddOption<NumberOption<int>>("G", nullptr, &g_WorldOptions->m_Green, 0, 255);
			sub->AddOption<NumberOption<int>>("B", nullptr, &g_WorldOptions->m_Blue, 0, 255);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Miscellaneous", SubmenuMisc, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Anti Oribtal Cannon", "Removes you from the selection list", &g_MiscOptions->m_HideOrbital, []
			{
				QUEUE()
				{
					if (g_MiscOptions->m_HideOrbital)
						NETWORK::SET_REMOTE_PLAYER_AS_GHOST(PLAYER::PLAYER_ID(), TRUE);
					else
						NETWORK::SET_REMOTE_PLAYER_AS_GHOST(PLAYER::PLAYER_ID(), FALSE);
				}
				STOP
			});
			/*sub->AddOption<BoolOption<bool>>("Disable FOG", nullptr, &g_MiscOptions->m_DisableFOG, []
			{
				if (!g_MiscOptions->m_DisableFOG) {
					auto Base = (std::uint64_t)GetModuleHandleA("GTA5.exe");
					*(float*)g_Functions->FindAddress(Base + 0x19B1D70, {}) = 0.00009999999747f;
				}
			});*/
			sub->AddOption<BoolOption<bool>>("Disable Phone", nullptr, &g_MiscOptions->m_DisablePhone, []
			{
				QUEUE() 
				{
					if (!g_MiscOptions->m_DisablePhone) {
						PAD::DISABLE_CONTROL_ACTION(0, 27, FALSE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Disable HUD", nullptr, &g_MiscOptions->m_DisableHUD, []
			{
				QUEUE()
				{
					if (!g_MiscOptions->m_DisableHUD) {
						HUD::DISPLAY_HUD(TRUE);
						HUD::DISPLAY_RADAR(TRUE);
					}
				}
				STOP
			});
			sub->AddOption<BoolOption<bool>>("Rage Quit", "Closes your game when you press ALT + F4", &g_MiscOptions->m_RageQuit);
			static float g_RadarZoom{ 1.f };
			sub->AddOption<NumberOption<float>>("Radar Zoom", nullptr, &g_RadarZoom, 0.f, 1.f, 1.f, 0, true, "", "", [] {
				QUEUE() { HUD::SET_RADAR_ZOOM(g_RadarZoom); } STOP
			});
			sub->AddOption<RegularOption>("Stop Cutscene", nullptr, [] { QUEUE() { CUTSCENE::STOP_CUTSCENE_IMMEDIATELY(); } STOP });
			sub->AddOption<RegularOption>("Bail", "Sends you to Story Mode", [] { QUEUE() { NETWORK::NETWORK_BAIL(0, 0, 0); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Teleport", SubmenuTeleport, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Camera Animation", nullptr, &g_TeleportOptions->m_CameraAnimation);
			sub->AddOption<BoolOption<bool>>("Keep Vehicle", nullptr, &g_TeleportOptions->m_KeepVehicle);
			sub->AddOption<RegularOption>("Waypoint", nullptr, [] { QUEUE() { g_TeleportOptions->ToWaypoint(); } STOP });
			sub->AddOption<RegularOption>("Objective", nullptr, [] { QUEUE() { g_TeleportOptions->ToObjective(); } STOP });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Recovery", SubmenuRecovery, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Money", nullptr, SubmenuMoney);
			sub->AddOption<SubOption>("Rank", nullptr, SubmenuRank);
			sub->AddOption<SubOption>("Unlocks", nullptr, SubmenuUnlocks);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Money", SubmenuMoney, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Nightclub Loop [$300K]", nullptr, &g_NetworkOptions->m_NightClubLoop, []
			{
				QUEUE()
				{
					if (g_NetworkOptions->m_NightClubLoop) 
					{
						*ScriptGlobal(262145).Add(24045).As<int*>() = 300000;
						*ScriptGlobal(262145).Add(24041).As<int*>() = 300000;
						g_Functions->SetStat("CLUB_POPULARITY", 10000);
						g_Functions->SetStat("CLUB_PAY_TIME_LEFT", -1);
						g_Functions->SetStat("CLUB_POPULARITY", 10000);
						STATS::STAT_SAVE(0, 0, 3, 0);
					}
				}
				STOP
			});
			sub->AddOption<ChooseBoolOption<const char*, std::size_t, bool>>("Internet Money", nullptr, &g_NetworkOptions->m_InternetMoney, &g_NetworkOptions->m_InternetMoneyMode, &g_NetworkOptions->m_InternetMoneyModePos, true, []
			{
				QUEUE()
				{
					if (g_NetworkOptions->m_InternetMoney)
					{
						*ScriptGlobal(1968313).As<int*>() = (g_NetworkOptions->m_InternetMoneyModePos + 1);
					}
				}
				STOP
			});
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Rank", SubmenuRank, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<int>>("Rank", nullptr, &g_NetworkOptions->m_Rank, 1, 8000);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Method", nullptr, &g_NetworkOptions->m_RankType, &g_NetworkOptions->m_RankTypePos);
			sub->AddOption<RegularOption>("Set Rank", nullptr, [] { g_Functions->SetRank(g_NetworkOptions->m_Rank, g_NetworkOptions->m_RankTypePos == 1); });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Unlocks", SubmenuUnlocks, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Unlock Misc", nullptr, [] { g_Functions->UnlockMisc(); });
			sub->AddOption<RegularOption>("Unlock Heist Vehicles", nullptr, [] { g_Functions->UnlockHeistVehicles(); });
			sub->AddOption<RegularOption>("Unlock Weapons", nullptr, [] { g_Functions->UnlockWeapons(); });
			sub->AddOption<RegularOption>("Unlock Clothes", nullptr, [] { g_Functions->UnlockClothes(); });
			sub->AddOption<RegularOption>("Unlock Haircuts", nullptr, [] { g_Functions->UnlockHaircuts(); });
			sub->AddOption<RegularOption>("Unlock Tattoos", nullptr, [] { g_Functions->UnlockTattoos(); });
			sub->AddOption<RegularOption>("Unlock Camos & Chutes", nullptr, [] { g_Functions->UnlockAllCamos(); });
			sub->AddOption<RegularOption>("Unlock LSC Mods", nullptr, [] { g_Functions->UnlockLSCMods(); });
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("X Position", nullptr, &g_UiManager->m_PosX, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("Y Position", nullptr, &g_UiManager->m_PosY, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<BoolOption<std::atomic_bool>>("Move With Mouse", nullptr, &g_Settings.m_LockMouse);
			sub->AddOption<BoolOption<bool>>("Sounds", nullptr, &g_UiManager->m_Sounds);
			sub->AddOption<RegularOption>("Unload", "Unload the menu", [] { g_Running = false; });
			sub->AddOption<RegularOption>("Exit", "Close the game", [] { raise(SIGSEGV); });
		});
	}

	void MainScript::Tick()
	{
		g_MainScript->Initialize();
		while (true)
		{
			g_UiManager->CheckForInput();
			g_UiManager->HandleInput();

			Script::GetCurrent()->ScriptYield();
		}
	}
}
