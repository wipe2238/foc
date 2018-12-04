#ifndef __ANIMATIONS__
#define __ANIMATIONS__

// Anim loading flags
#define ANIM_FLAG_FIRST_FRAME        (0x01)
#define ANIM_FLAG_LAST_FRAME         (0x02)
	// Manage constants
#define ANIM1_WEAPON_MASK            (0x000000FF)
	// Weapons
#define ANIM1_UNARMED                (1) // Hardcoded
#define ANIM1_KNIFE                  (4)
#define ANIM1_CLUB                   (5)
#define ANIM1_HAMMER                 (6)
#define ANIM1_SPEAR                  (7)
#define ANIM1_PISTOL                 (8)
#define ANIM1_SMG                    (9)
#define ANIM1_SHOOTGUN               (10)
#define ANIM1_HEAVY_RIFLE            (11)
#define ANIM1_MINIGUN                (12)
#define ANIM1_ROCKET_LAUNCHER        (13)
#define ANIM1_FLAMER                 (14)
#define ANIM1_RIFLE                  (15)
#define ANIM1_SWORD                  (16)
#define ANIM1_LONG_SWORD             (17)
#define ANIM1_AXE                    (18)
#define ANIM1_BOW                    (19)

// Anim2 Actions
	// Manage constants
#define ANIM2_DEAD_BEGIN             (100)
#define ANIM2_DEAD_END               (120)
	// Animations
#define ANIM2_IDLE                   (1)  // Hardcoded
#define ANIM2_IDLE_STUNNED           (2)
#define ANIM2_WALK                   (3)  // Hardcoded
#define ANIM2_LIMP                   (4)  // Hardcoded
#define ANIM2_RUN                    (5)  // Hardcoded
#define ANIM2_PANIC_RUN              (6)  // Hardcoded
#define ANIM2_SNEAK_WALK             (7)  // Hardcoded
#define ANIM2_SNEAK_RUN              (8)  // Hardcoded
#define ANIM2_STAND                  (10)
#define ANIM2_CROUCH                 (11)
#define ANIM2_PRONE                  (12)
#define ANIM2_SHOW_WEAPON            (20)
#define ANIM2_HIDE_WEAPON            (21)
#define ANIM2_PREPARE_WEAPON         (22)
#define ANIM2_TURNOFF_WEAPON         (23)
#define ANIM2_FIDGET                 (24)
#define ANIM2_CLIMBING               (26)
#define ANIM2_PICKUP                 (27)
#define ANIM2_USE                    (28)
#define ANIM2_SWITCH_ITEMS           (29)
#define ANIM2_RELOAD                 (30)
#define ANIM2_REPAIR                 (31)
#define ANIM2_LOOT                   (35)
#define ANIM2_STEAL                  (36)
#define ANIM2_PUSH                   (37)
#define ANIM2_BEGIN_COMBAT           (40)
#define ANIM2_IDLE_COMBAT            (41)
#define ANIM2_END_COMBAT             (42)
#define ANIM2_PUNCH_RIGHT            (43)
#define ANIM2_PUNCH_LEFT             (44)
#define ANIM2_PUNCH_COMBO            (45)
#define ANIM2_KICK_HI                (46)
#define ANIM2_KICK_LO                (47)
#define ANIM2_KICK_COMBO             (48)
#define ANIM2_THRUST_1H              (49)
#define ANIM2_THRUST_2H              (50)
#define ANIM2_SWING_1H               (51)
#define ANIM2_SWING_2H               (52)
#define ANIM2_THROW                  (53)
#define ANIM2_SINGLE                 (54)
#define ANIM2_BURST                  (55)
#define ANIM2_SWEEP                  (56)
#define ANIM2_BUTT                   (57)
#define ANIM2_FLAME                  (58)
#define ANIM2_NO_RECOIL              (59)
#define ANIM2_DODGE_FRONT            (70)
#define ANIM2_DODGE_BACK             (71)
#define ANIM2_DAMAGE_FRONT           (72)
#define ANIM2_DAMAGE_BACK            (73)
#define ANIM2_DAMAGE_MUL_FRONT       (74)
#define ANIM2_DAMAGE_MUL_BACK        (75)
#define ANIM2_WALK_DAMAGE_FRONT      (76)
#define ANIM2_WALK_DAMAGE_BACK       (77)
#define ANIM2_LIMP_DAMAGE_FRONT      (78)
#define ANIM2_LIMP_DAMAGE_BACK       (79)
#define ANIM2_RUN_DAMAGE_FRONT       (80)
#define ANIM2_RUN_DAMAGE_BACK        (81)
#define ANIM2_KNOCK_FRONT            (82)
#define ANIM2_KNOCK_BACK             (83)
#define ANIM2_LAYDOWN_FRONT          (84)
#define ANIM2_LAYDOWN_BACK           (85)
#define ANIM2_IDLE_PRONE_FRONT       (86) // Hardcoded
#define ANIM2_IDLE_PRONE_BACK        (87) // Hardcoded
#define ANIM2_STANDUP_FRONT          (88)
#define ANIM2_STANDUP_BACK           (89)
#define ANIM2_DAMAGE_PRONE_FRONT     (90)
#define ANIM2_DAMAGE_PRONE_BACK      (91)
#define ANIM2_DAMAGE_MUL_PRONE_FRONT (92)
#define ANIM2_DAMAGE_MUL_PRONE_BACK  (93)
#define ANIM2_TWITCH_PRONE_FRONT     (94)
#define ANIM2_TWITCH_PRONE_BACK      (95)
#define ANIM2_DEAD_PRONE_FRONT       (100)
#define ANIM2_DEAD_PRONE_BACK        (101)
#define ANIM2_DEAD_FRONT             (102) // Hardcoded
#define ANIM2_DEAD_BACK              (103) // Hardcoded
#define ANIM2_DEAD_BLOODY_SINGLE     (110)
#define ANIM2_DEAD_BLOODY_BURST      (111)
#define ANIM2_DEAD_BURST             (112)
#define ANIM2_DEAD_PULSE             (113)
#define ANIM2_DEAD_PULSE_DUST        (114)
#define ANIM2_DEAD_LASER             (115)
#define ANIM2_DEAD_FUSED             (116)
#define ANIM2_DEAD_EXPLODE           (117)
#define ANIM2_DEAD_BURN              (118)
#define ANIM2_DEAD_BURN_RUN          (119)

// Fallout animations
#define ANIM1_FALLOUT_UNARMED            (1)
#define ANIM1_FALLOUT_DEAD               (2)
#define ANIM1_FALLOUT_KNOCKOUT           (3)
#define ANIM1_FALLOUT_KNIFE              (4)
#define ANIM1_FALLOUT_CLUB               (5)
#define ANIM1_FALLOUT_HAMMER             (6)
#define ANIM1_FALLOUT_SPEAR              (7)
#define ANIM1_FALLOUT_PISTOL             (8)
#define ANIM1_FALLOUT_UZI                (9)
#define ANIM1_FALLOUT_SHOOTGUN           (10)
#define ANIM1_FALLOUT_RIFLE              (11)
#define ANIM1_FALLOUT_MINIGUN            (12)
#define ANIM1_FALLOUT_ROCKET_LAUNCHER    (13)
#define ANIM1_FALLOUT_AIM                (14)
#define ANIM2_FALLOUT_STAY               (1)
#define ANIM2_FALLOUT_WALK               (2)
#define ANIM2_FALLOUT_SHOW               (3)
#define ANIM2_FALLOUT_HIDE               (4)
#define ANIM2_FALLOUT_DODGE_WEAPON       (5)
#define ANIM2_FALLOUT_THRUST             (6)
#define ANIM2_FALLOUT_SWING              (7)
#define ANIM2_FALLOUT_PREPARE_WEAPON     (8)
#define ANIM2_FALLOUT_TURNOFF_WEAPON     (9)
#define ANIM2_FALLOUT_SHOOT              (10)
#define ANIM2_FALLOUT_BURST              (11)
#define ANIM2_FALLOUT_FLAME              (12)
#define ANIM2_FALLOUT_THROW_WEAPON       (13)
#define ANIM2_FALLOUT_DAMAGE_FRONT       (15)
#define ANIM2_FALLOUT_DAMAGE_BACK        (16)
#define ANIM2_FALLOUT_KNOCK_FRONT        (1)  // Only with ANIM1_FALLOUT_DEAD
#define ANIM2_FALLOUT_KNOCK_BACK         (2)
#define ANIM2_FALLOUT_STANDUP_BACK       (8)  // Only with ANIM1_FALLOUT_KNOCKOUT
#define ANIM2_FALLOUT_STANDUP_FRONT      (10)
#define ANIM2_FALLOUT_PICKUP             (11) // Only with ANIM1_FALLOUT_UNARMED
#define ANIM2_FALLOUT_USE                (12)
#define ANIM2_FALLOUT_DODGE_EMPTY        (14)
#define ANIM2_FALLOUT_PUNCH              (17)
#define ANIM2_FALLOUT_KICK               (18)
#define ANIM2_FALLOUT_THROW_EMPTY        (19)
#define ANIM2_FALLOUT_RUN                (20)
#define ANIM2_FALLOUT_DEAD_FRONT         (1)  // Only with ANIM1_FALLOUT_DEAD
#define ANIM2_FALLOUT_DEAD_BACK          (2)
#define ANIM2_FALLOUT_DEAD_BLOODY_SINGLE (4)
#define ANIM2_FALLOUT_DEAD_BURN          (5)
#define ANIM2_FALLOUT_DEAD_BLOODY_BURST  (6)
#define ANIM2_FALLOUT_DEAD_BURST         (7)
#define ANIM2_FALLOUT_DEAD_PULSE         (8)
#define ANIM2_FALLOUT_DEAD_LASER         (9)
#define ANIM2_FALLOUT_DEAD_BURN2         (10)
#define ANIM2_FALLOUT_DEAD_PULSE_DUST    (11)
#define ANIM2_FALLOUT_DEAD_EXPLODE       (12)
#define ANIM2_FALLOUT_DEAD_FUSED         (13)
#define ANIM2_FALLOUT_DEAD_BURN_RUN      (14)
#define ANIM2_FALLOUT_DEAD_FRONT2        (15)
#define ANIM2_FALLOUT_DEAD_BACK2         (16)

class Anim
{
public:
	void ProcessAction(bool localCall, CritterCl& cr, int action, int actionExt, ItemCl* item);
	bool ProcessFallout(uint crType, uint& anim1, uint& anim2, uint& anim1ex, uint& anim2ex, uint& flags);
//
	void ClearAnim(CritterCl& cr);
	bool IsAnimAvailable(CritterCl& cr, uint anim1, uint anim2);
	void Animate(CritterCl& cr, uint anim1, uint anim2);
	void Animate(CritterCl& cr, uint anim1, uint anim2, ItemCl* item);
};

extern Anim Animations;

#endif // __ANIMATIONS__ //
