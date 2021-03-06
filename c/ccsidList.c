//typedef struct CCSID_ID {
typedef struct{
  int  ccsid;
  char *idName;
  } CCSID_ID;

CCSID_ID ccsidList[] = {
  { 37, "IBM-037"},
  {273, "IBM-273"},
  {274, "IBM-274"},
  {275, "IBM-275"},
  {277, "IBM-277"},
  {278, "IBM-278"},
  {280, "IBM-280"},
  {281, "IBM-281"},
  {282, "IBM-282"},
  {284, "IBM-284"},
  {285, "IBM-285"},
  {290, "IBM-290"},
  {297, "IBM-297"},
  {300, "IBM-300"},
  {301, "IBM-301"},
  {420, "IBM-420"},
  {424, "IBM-424"},
  {425, "IBM-425"},
  {437, "IBM-437"},
  {500, "IBM-500"},
  {808, "IBM-808"},
  {813, "ISO8859-7"},
  {819, "ISO8859-1"},
  {833, "IBM-833"},
  {834, "IBM-834"},
  {835, "IBM-835"},
  {836, "IBM-836"},
  {837, "IBM-837"},
  {838, "IBM-838"},
  {848, "IBM-848"},
  {850, "IBM-850"},
  {852, "IBM-852"},
  {855, "IBM-855"},
  {856, "IBM-856"},
  {858, "IBM-858"},
  {859, "IBM-859"},
  {861, "IBM-861"},
  {862, "IBM-862"},
  {864, "IBM-864"},
  {866, "IBM-866"},
  {867, "IBM-867"},
  {869, "IBM-869"},
  {870, "IBM-870"},
  {871, "IBM-871"},
  {872, "IBM-872"},
  {874,  "TIS-620"},
  {875, "IBM-875"},
  {880, "IBM-880"},
  {901, "IBM-901"},
  {902, "IBM-902"},
  {904, "IBM-904"},
  {912, "ISO8859-2"},
  {914, "ISO8859-4"},
  {915, "ISO8859-5"},
  {916, "ISO8859-8"},
  {920, "ISO8859-9"},
  {921, "ISO8859-13"},
  {922, "IBM-922"},
  {923, "ISO8859-15"},
  {924, "IBM-924"},
  {927, "IBM-927"},
  {928, "IBM-928"},
  {930, "IBM-930"},
  {932, "IBM-eucJC"},
  {933, "IBM-933"},
  {935, "IBM-935"},
  {936, "IBM-936"},
  {937, "IBM-937"},
  {938, "IBM-938"},
  {939, "IBM-939"},
  {942, "IBM-942"},
  {943, "IBM-943"},
  {946, "IBM-946"},
  {947, "IBM-947"},
  {948, "IBM-948"},
  {949, "IBM-949"},
  {950, "BIG5"},
  {951, "IBM-951"},
  {956, "IBM-956"},
  {957, "IBM-957"},
  {958, "IBM-958"},
  {959, "IBM-959"},
  {964, "IBM-eucTW"},
  {970, "IBM-eucKR"},
  {1025, "IBM-1025"},
  {1026, "IBM-1026"},
  {1027, "IBM-1027"},
  {1046, "IBM-1046"},
  {1047, "IBM-1047"},
  {1088, "IBM-1088"},
  {1089, "ISO8859-6"},
  {1112, "IBM-1112"},
  {1115, "IBM-1115"},
  {1122, "IBM-1122"},
  {1123, "IBM-1123"},
  {1124, "IBM-1124"},
  {1125, "IBM-1125"},
  {1126, "IBM-1126"},
  {1140, "IBM-1140"},
  {1141, "IBM-1141"},
  {1142, "IBM-1142"},
  {1143, "IBM-1143"},
  {1144, "IBM-1144"},
  {1145, "IBM-1145"},
  {1146, "IBM-1146"},
  {1147, "IBM-1147"},
  {1148, "IBM-1148"},
  {1149, "IBM-1149"},
  {1153, "IBM-1153"},
  {1154, "IBM-1154"},
  {1155, "IBM-1155"},
  {1156, "IBM-1156"},
  {1157, "IBM-1157"},
  {1158, "IBM-1158"},
  {1159, "IBM-1159"},
  {1160, "IBM-1160"},
  {1161, "IBM-1161"},
  {1165, "IBM-1165"},
  {1208, "UTF-8"},
  {1250, "IBM-1250"},
  {1251, "IBM-1251"},
  {1252, "IBM-1252"},
  {1253, "IBM-1253"},
  {1254, "IBM-1254"},
  {1255, "IBM-1255"},
  {1256, "IBM-1256"},
  {1362, "IBM-1362"},
  {1363, "IBM-1363"},
  {1364, "IBM-1364"},
  {1370, "IBM-1370"},
  {1371, "IBM-1371"},
  {1380, "IBM-1380"},
  {1381, "IBM-1381"},
  {1383, "IBM-eucCN"},
  {1386, "IBM-1386"},
  {1388, "IBM-1388"},
  {1390, "IBM-1390"},
  {1399, "IBM-1399"},
  {4396, "IBM-4396"},
  {4909, "IBM-4909"},
  {4930, "IBM-4930"},
  {4933, "IBM-4933"},
  {4946, "IBM-4946"},
  {4971, "IBM-4971"},
  {5026, "IBM-5026"},
  {5031, "IBM-5031"},
  {5035, "IBM-5035"},
  {5052, "ISO-2022-JP"},
  {5053, "IBM-5053"},
  {5054, "IBM-5054"},
  {5055, "IBM-5055"},
  {5123, "IBM-5123"},
  {5346, "IBM-5346"},
  {5347, "IBM-5347"},
  {5348, "IBM-5348"},
  {5349, "IBM-5349"},
  {5350, "IBM-5350"},
  {5351, "IBM-5351"},
  {5352, "IBM-5352"},
  {5488, "IBM-5488"},
  {8482, "IBM-8482"},
  {9027, "IBM-9027"},
  {9044, "IBM-9044"},
  {9061, "IBM-9061"},
  {9238, "IBM-9238"},
  {12712, "IBM-12712"},
  {13121, "IBM-13121"},
  {13124, "IBM-13124"},
  {13488, "UCS-2"},
  {16684, "IBM-16684"},
  {16804, "IBM-16804"},
  {17248, "IBM-17248"},
  {28709, "IBM-28709"},
  {53668, "IBM-53668"},
  {54191, "IBM-54191"},
  {62383, "IBM-62383"},
  {    0, NULL}
};
