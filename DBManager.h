#pragma once

#include "index.h"
#include "line.h"
#include "bTree.h"
#include "tab.h"
#include "SQL.h"

#define ROOT_PATH		 "E:\\DBfile\\"
#define TAB_PATH		 "E:\\DBfile\\tab\\"
#define RAWTAB_PATH      "E:\\DBfile\\rawTab\\"
#define PK_DEFAULT_POS    0
#define MAX_ATTR_BUFFER    10
#define MAX_DATA_BUFFER    1000

enum SEARCH_MODE { EQUAL, NOT_EQUAL, GREATER, SMALLER, REGION };