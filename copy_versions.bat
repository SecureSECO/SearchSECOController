copy "VERSION" "x64/Release/VERSION"

mkdir "x64/Release/spider"
mkdir "x64/Release/parser"

more "spider/VERSION" > "x64/Release/spider/VERSION"
more "parser/VERSION" > "x64/Release/parser/VERSION"