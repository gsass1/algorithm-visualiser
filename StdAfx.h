#pragma once

#include <algorithm>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include <future>

#include "ICommon.h"
#include "IGraphics.h"
#include "IInput.h"
#include "IResourceManager.h"
#include "IFilesystem.h"
#include "ISoundSystem.h"

#include "StringUtils.h"
#include "IFont.h"
#include "Vector2.h"
#include "Rect.h"
#include "Color.h"
#include "MathUtil.h"

#undef min
#undef max