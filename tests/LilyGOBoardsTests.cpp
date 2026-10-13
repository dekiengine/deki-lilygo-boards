// The boards this package ships, read the way the editor reads them: every
// platforms/<id>/<id>.json parses, names itself after its folder, points at a
// boot scene that is there, describes a screen, asks only for packages this
// package requires, and passes the ESP-IDF builder's own checks (the chip and
// display bus reach a shell line, so a typo is refused there, not in a build
// log). A board added later is covered without a change here.
//
// The boot scenes themselves are checked by deki-editor's
// ShippedPlatformTemplates tests.

#include <gtest/gtest.h>

#include "editor/ESPIDFBuilder.h"

#include <deki-editor/build/PlatformConfig.h>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace DekiEditor;
namespace fs = std::filesystem;

namespace
{

const fs::path kPackage = DEKI_LILYGO_BOARDS_DIR;

std::string Read(const fs::path& p)
{
    std::ifstream f(p, std::ios::binary);
    std::ostringstream s;
    s << f.rdbuf();
    return s.str();
}

std::vector<fs::path> BoardFiles()
{
    std::vector<fs::path> files;
    for (const auto& dir : fs::directory_iterator(kPackage / "platforms"))
    {
        if (dir.is_directory())
        {
            files.push_back(dir.path() / (dir.path().filename().string() + ".json"));
        }
    }
    std::sort(files.begin(), files.end());
    return files;
}

std::vector<std::string> PackageRequires()
{
    const auto j = nlohmann::json::parse(Read(kPackage / "package.json"));
    return j.value("requires", std::vector<std::string>{});
}

}  // namespace

TEST(LilyGOBoards, ThereAreBoards)
{
    EXPECT_GE(BoardFiles().size(), 2u);
}

TEST(LilyGOBoards, EveryBoardLoadsAndPassesTheBuildersChecks)
{
    ESPIDFBuilder builder;
    const std::vector<std::string> requires_ = PackageRequires();
    for (const fs::path& file : BoardFiles())
    {
        SCOPED_TRACE(file.string());
        ASSERT_TRUE(fs::exists(file)) << "a board folder needs <id>/<id>.json";

        PlatformConfig config;
        ASSERT_TRUE(PlatformFromJson(Read(file), config));
        EXPECT_EQ(config.id, file.parent_path().filename().string());
        EXPECT_FALSE(config.displayName.empty());
        EXPECT_EQ(config.framework, "espidf");

        const std::vector<std::string> problems = builder.ValidatePlatform(config);
        for (const auto& p : problems)
        {
            ADD_FAILURE() << p;
        }

        EXPECT_GT(config.screenWidth, 0);
        EXPECT_GT(config.screenHeight, 0);
        EXPECT_TRUE(config.colorFormat == "RGB565" || config.colorFormat == "RGB888" ||
                    config.colorFormat == "RGBA8888")
            << config.colorFormat;

        ASSERT_FALSE(config.bootScene.empty());
        EXPECT_TRUE(fs::exists(kPackage / config.bootScene)) << config.bootScene;

        for (const auto& pkg : config.requiresPackages)
        {
            EXPECT_NE(std::find(requires_.begin(), requires_.end(), pkg), requires_.end())
                << pkg << " is asked for by the board but not required by the package";
        }
    }
}
