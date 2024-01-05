#pragma once

#include "core/Screen.h"

const Frame framesGhzGim[] = {
    Frame(IntRect(0,0,48,32)),  //Stone
    Frame(IntRect(48,0,96,88), Vector2i(48, 56)), //Slp platform
    Frame(IntRect(0,32,16,16)), //Bridge
    Frame(IntRect(0,90,64,30)), //Platform

    Frame(IntRect(144,0,16,64)), //Wall left shadow
    Frame(IntRect(160,0,16,64)), //Wall left
    Frame(IntRect(144,64,16,64)), //Wall right

    Frame(IntRect(64,88,32, 16)), //Bridge column
};

const Frame framesObjects[] = {
    //Animation sIdle 0 - 3
    Frame(IntRect(0,0,29,39),  Vector2i(14, 19)),
    Frame(IntRect(29,1,30,38), Vector2i(12,18)),
    Frame(IntRect(59,1,30,38), Vector2i(12,18)),
    Frame(IntRect(89,1,29,38), Vector2i(12,18)),
    //Animation,sWalk 4 - 9
    Frame(IntRect(147,2,24,37), Vector2i(13,17)),
    Frame(IntRect(171,2,36,37), Vector2i(19,19)),
    Frame(IntRect(207,1,29,38), Vector2i(18,18)),
    Frame(IntRect(236,1,27,37), Vector2i(14,17)),
    Frame(IntRect(263,1,37,37), Vector2i(18,19)),
    Frame(IntRect(300,0,32,38), Vector2i(14,18)),
    //Animation,sSit 10
    Frame(IntRect(118,13,29,26),Vector2i(9,6)),
    //Animation,sRun 11 - 14
    Frame(IntRect(332,1,31,35), Vector2i(20,15)),
    Frame(IntRect(363,0,31,36), Vector2i(20,16)),
    Frame(IntRect(394,1,30,36), Vector2i(19,15)),
    Frame(IntRect(424,0,31,36), Vector2i(20,16)),
    //Animation,sRoll 15 - 22
    Frame(IntRect(456,1,30,30), Vector2i(15,15)),
    Frame(IntRect(488,1,29,30), Vector2i(15,16)),
    Frame(IntRect(456,1,30,30), Vector2i(15,15)),
    Frame(IntRect(520,1,30,29), Vector2i(16,16)),
    Frame(IntRect(456,1,30,30), Vector2i(15,15)),
    Frame(IntRect(553,1,29,30), Vector2i(15,16)),
    Frame(IntRect(456,1,30,30), Vector2i(15,15)),
    Frame(IntRect(584,2,30,29), Vector2i(16,15)),
    //Animation,sSkid 23 - 25
    Frame(IntRect(615,0,30,35), Vector2i(12,15)),
    Frame(IntRect(645,0,31,36), Vector2i(13,16)),
    Frame(IntRect(681,37,33,36), Vector2i(15,16)),
    //Animation,sDiaWalk 26 - 31
    Frame(IntRect(680,0,34,37), Vector2i(17,18)),
    Frame(IntRect(718,0,36,43), Vector2i(18,19)),
    Frame(IntRect(758,0,35,40), Vector2i(17,18)),
    Frame(IntRect(795,2,38,38), Vector2i(18,16)),
    Frame(IntRect(835,2,38,43), Vector2i(19,17)),
    Frame(IntRect(875,2,41,34), Vector2i(18,16)),
    //Animation,sDiaRun 32 - 35
    Frame(IntRect(919,1,37,39), Vector2i(19,17)),
    Frame(IntRect(959,0,36,40), Vector2i(18,18)),
    Frame(IntRect(2,40,36,39), Vector2i(19,17)),
    Frame(IntRect(43,39,36,40), Vector2i(18,18)),
    //Animation,sHurt 36
    Frame(IntRect(80,39,40,28), Vector2i(20,14)),
    //Animation,sDie 37
    Frame(IntRect(120,39,34,43), Vector2i(17,21)),
    //Animation,sPush 38 - 41
    Frame(IntRect(154,40,29,36), Vector2i(14,17)),
    Frame(IntRect(183,39,24,37), Vector2i(9,18)),
    Frame(IntRect(207,40,28,36), Vector2i(13,17)),
    Frame(IntRect(235,39,24,37), Vector2i(9,18)),
    //Animation,sLookUp 42
    Frame(IntRect(259,40,28,38), Vector2i(14,18)),
    //Animation,sDash 43 - 46
    Frame(IntRect(287,40,39,31), Vector2i(28,11)),
    Frame(IntRect(327,39,39,32), Vector2i(28,12)),
    Frame(IntRect(367,37,39,31), Vector2i(28,11)),
    Frame(IntRect(408,36,38,32), Vector2i(27,12)),
    //Animation,sDiaDash 47 - 50
    Frame(IntRect(449,37,37,41), Vector2i(18,14)),
    Frame(IntRect(489,32,34,40), Vector2i(18,14)),
    Frame(IntRect(530,32,37,43), Vector2i(18,14)),
    Frame(IntRect(569,33,35,39), Vector2i(18,14)),
    //Animation,sSpring 51 - 53
    Frame(IntRect(607,36,25,43), Vector2i(13,22)),
    Frame(IntRect(632,36,20,44), Vector2i(8,22)),
    Frame(IntRect(652,37,29,44), Vector2i(14,22)),
    //Animation,sFall 54 - 55
    Frame(IntRect(714,43,36,48), Vector2i(18,24)),
    Frame(IntRect(750,43,36,47), Vector2i(18,23)),
    //Animation,Monitor 56 - 57
    Frame(IntRect(786,40,30,30), Vector2i(15,15)),
    Frame(IntRect(816,45,32,16), Vector2i(16,1)),
    //Animation,MonitorIcon 58 - 64
    Frame(IntRect(786,70,16,14), Vector2i(8,7)),
    Frame(IntRect(802,70,16,14), Vector2i(8,7)),
    Frame(IntRect(818,61,16,14), Vector2i(8,7)),
    Frame(IntRect(834,61,16,14), Vector2i(8,7)),
    Frame(IntRect(850,61,16,14), Vector2i(8,7)),
    Frame(IntRect(818,75,16,14), Vector2i(8,7)),
    Frame(IntRect(834,75,16,14), Vector2i(8,7)),
    //Animation,Springs 65 - 68
    Frame(IntRect(848,45,28,16), Vector2i(14,8)),
    Frame(IntRect(866,61,28,32), Vector2i(14,24)),
    Frame(IntRect(876,37,28,16), Vector2i(14,8)),
    Frame(IntRect(894,61,28,32), Vector2i(14,24)),
    //Animation,KillScore 69 - 74
    Frame(IntRect(876,53,9,8), Vector2i(4,4)),
    Frame(IntRect(885,53,14,8), Vector2i(7,4)),
    Frame(IntRect(904,37,15,8), Vector2i(7,4)),
    Frame(IntRect(918,53,15,8), Vector2i(7,4)),
    Frame(IntRect(899,53,19,8), Vector2i(9,4)),
    Frame(IntRect(904,45,24,8), Vector2i(12,4)),
    //Animation,Act 75 - 79
    Frame(IntRect(928,40,24,8), Vector2i(12,4)),
    Frame(IntRect(968,40,16,16), Vector2i(8,8)),
    Frame(IntRect(933,48,7,24), Vector2i(3,12)),
    Frame(IntRect(940,48,14,24), Vector2i(7,12)),
    Frame(IntRect(954,48,14,24), Vector2i(7,12)),
    //Animation,Ring 80 - 87
    Frame(IntRect(984,40,16,16), Vector2i(8,8)),
    Frame(IntRect(968,56,12,16), Vector2i(6,8)),
    Frame(IntRect(980,56,6,16), Vector2i(3,8)),
    Frame(IntRect(986,56,12,16), Vector2i(6,8)),
    Frame(IntRect(935,72,16,14), Vector2i(8,7)),
    Frame(IntRect(951,72,16,14), Vector2i(8,7)),
    Frame(IntRect(967,72,16,14), Vector2i(8,7)),
    Frame(IntRect(983,72,16,14), Vector2i(8,7)),
    //Animation,SkidDust 88 - 91
    Frame(IntRect(1000,32,13,14), Vector2i(6,7)),
    Frame(IntRect(1000,46,11,12), Vector2i(5,6)),
    Frame(IntRect(1001,58,11,9), Vector2i(5,4)),
    Frame(IntRect(1002,67,8,10), Vector2i(4,5)),
    //Animation,Flicky 92 - 94
    Frame(IntRect(850,75,11,20), Vector2i(5,10)),
    Frame(IntRect(786,84,17,15), Vector2i(8,7)),
    Frame(IntRect(803,85,15,14), Vector2i(7,7)),
    //Animation,Explosion 95 - 99
    Frame(IntRect(1166,0,20,16), Vector2i(10,8)),
    Frame(IntRect(1165,16,26,30), Vector2i(13,15)),
    Frame(IntRect(1168,46,33,32), Vector2i(16,16)),
    Frame(IntRect(1191,0,38,38), Vector2i(19,19)),
    Frame(IntRect(1202,38,36,38),Vector2i(18,19)),
    //Animation,Invisible 100 - 103
    Frame(IntRect(1238,1,48,47), Vector2i(24,23)),
    Frame(IntRect(1286,0,48,47), Vector2i(24,23)),
    Frame(IntRect(1241,48,46,460), Vector2i(23,23)),
    Frame(IntRect(1288,48,46,46), Vector2i(23,23)),
    //Animation,SignPost 104 - 109
    Frame(IntRect(1406,64,6,16),  Vector2i(3,8)),
    Frame(IntRect(1390,0,48,32),  Vector2i(24,16)),
    Frame(IntRect(1334,56,32,32), Vector2i(16,16)),
    Frame(IntRect(1398,64,8,32),  Vector2i(4,16)),
    Frame(IntRect(1366,64,32,32), Vector2i(16,16)),
    Frame(IntRect(1390,32,48,32), Vector2i(24,16)),
    //Animation,ActTitleThing 110
    Frame(IntRect(1334,0,56,56), Vector2i(28,28)),
    //Animation,Chopper 111 - 112
    Frame(IntRect(1090,0,30,32), Vector2i(15,16)),
    Frame(IntRect(1088,62,29,31), Vector2i(14,16)),
    //Animation,Crabmeat 113 - 116
    Frame(IntRect(1120,0,42,31), Vector2i(21,15)),
    Frame(IntRect(1120,31,44,31), Vector2i(22,15)),
    Frame(IntRect(1120,0,42,31), Vector2i(21,15)),
    Frame(IntRect(1120,62,48,30), Vector2i(24,14)),
    //Animation,buzzBomber 117 - 124
    Frame(IntRect(670,81,44,19), Vector2i(22,9)),
    Frame(IntRect(1046,64,36,29), Vector2i(18,14)),
    Frame(IntRect(714,91,35,8), Vector2i(17,4)),
    Frame(IntRect(749,91,37,6), Vector2i(18,3)),
    Frame(IntRect(681,76,6,5), Vector2i(3,2)),
    Frame(IntRect(687,76,10,5), Vector2i(5,2)),
    Frame(IntRect(640,82,16,16), Vector2i(8,8)),
    Frame(IntRect(656,82,14,14), Vector2i(7,7)),
    //Animation,bullet 125 - 127
    Frame(IntRect(1010,65,12,12), Vector2i(6,6)),
    Frame(IntRect(1022,65,12,12), Vector2i(6,6)),
    Frame(IntRect(1034,65,12,12), Vector2i(6,6)),
    //Animation,Debug 128 - 133
    Frame(IntRect(0,84,16,16), Vector2i(8,8)),
    Frame(IntRect(16,84,16,16), Vector2i(8,8)),
    Frame(IntRect(32,84,16,16), Vector2i(8,8)),
    Frame(IntRect(48,84,16,16), Vector2i(8,8)),
    Frame(IntRect(64,84,16,16), Vector2i(8,8)),
    Frame(IntRect(80,84,16,16), Vector2i(8,8)),
    // Anim Motobug 134 - 137
    Frame(IntRect(1012, 0, 39, 29), Vector2i(19, 14)),
    Frame(IntRect(1051, 1, 39, 28), Vector2i(19, 13)),
    Frame(IntRect(1013, 29, 40, 29), Vector2i(20, 14)),
    Frame(IntRect(1053, 32, 40, 28), Vector2i(20, 13)),
    // Spike 138
    Frame(IntRect(1001, 0, 8, 32)),
};