copy "VERSION" "x64/Release/VERSION"

mkdir "x64/Release/spider"
mkdir "x64/Release/parser"
mkdir "x64/Release/crawler"

more "spider-revisited/VERSION" > "x64/Release/spider/VERSION"
more "parser/VERSION" > "x64/Release/parser/VERSION"
more "crawler/VERSION" > "x64/Release/crawler/VERSION"

copy "VERSION" "x64/Debug/VERSION"

mkdir "x64/Debug/spider"
mkdir "x64/Debug/parser"
mkdir "x64/Debug/crawler"

more "spider-revisited/VERSION" > "x64/Debug/spider/VERSION"
more "parser/VERSION" > "x64/Debug/parser/VERSION"
more "crawler/VERSION" > "x64/Debug/crawler/VERSION"