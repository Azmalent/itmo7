rm -rf ./Tests/TestResults

dotnet test --collect:"XPlat Code Coverage" --settings runsettings 

cd Tests
dotnet reportgenerator -reports:'TestResults/*/coverage.cobertura.xml' -reporttypes:'Html' -targetdir:'./Report/'
cd ..

rm -rf ./Tests/TestResults
