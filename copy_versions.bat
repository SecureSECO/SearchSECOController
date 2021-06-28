copy "VERSION" "x64/Release/VERSION"

mkdir "x64/Release/SearchSECOSpider"
mkdir "x64/Release/SearchSECOParser"
mkdir "x64/Release/SearchSECOCrawler"

more "SearchSECOSpider/VERSION" > "x64/Release/SearchSECOSpider/VERSION"
more "SearchSECOParser/VERSION" > "x64/Release/SearchSECOParser/VERSION"
more "SearchSECOCrawler/VERSION" > "x64/Release/SearchSECOCrawler/VERSION"

copy "VERSION" "x64/Debug/VERSION"

mkdir "x64/Debug/SearchSECOSpider"
mkdir "x64/Debug/SearchSECOParser"
mkdir "x64/Debug/crawler"

more "SearchSECOSpider/VERSION" > "x64/Debug/SearchSECOSpider/VERSION"
more "SearchSECOParser/VERSION" > "x64/Debug/SearchSECOParser/VERSION"
more "SearchSECOCrawler/VERSION" > "x64/Debug/SearchSECOCrawler/VERSION"