
curl https://www.hepdata.net/download/table/ins1848440/Figure%205-1/1/yaml --output CMS-R02.yaml
curl https://www.hepdata.net/download/table/ins1848440/Figure%205-2/1/yaml --output CMS-R03.yaml
curl https://www.hepdata.net/download/table/ins1848440/Figure%205-3/1/yaml --output CMS-R04.yaml
curl https://www.hepdata.net/download/table/ins1848440/Figure%205-4/1/yaml --output CMS-R06.yaml
curl https://www.hepdata.net/download/table/ins1848440/Figure%205-5/1/yaml --output CMS-R08.yaml
curl https://www.hepdata.net/download/table/ins1848440/Figure%205-6/1/yaml --output CMS-R10.yaml

python3 ParseCMS.py 02
python3 ParseCMS.py 03
python3 ParseCMS.py 04
python3 ParseCMS.py 06
python3 ParseCMS.py 08
python3 ParseCMS.py 10

#cat CMS-R02.txt | ./TextToTree CMS-R02.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
#cat CMS-R03.txt | ./TextToTree CMS-R03.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
#cat CMS-R04.txt | ./TextToTree CMS-R04.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
#cat CMS-R06.txt | ./TextToTree CMS-R06.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
#cat CMS-R08.txt | ./TextToTree CMS-R08.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
#cat CMS-R10.txt | ./TextToTree CMS-R10.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"

cat CMS-R02.txt | ./Execute CMS-R02.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
cat CMS-R03.txt | ./Execute CMS-R03.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
cat CMS-R04.txt | ./Execute CMS-R04.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
cat CMS-R06.txt | ./Execute CMS-R06.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
cat CMS-R08.txt | ./Execute CMS-R08.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
cat CMS-R10.txt | ./Execute CMS-R10.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"

curl https://www.hepdata.net/download/table/ins1673184/Table%204/1/yaml --output ATLAS-R04.yaml

python3 ParseATLAS.py

#cat ATLAS-R04.txt | ./TextToTree ATLAS-R04.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"
cat ATLAS-R04.txt | ./Execute ATLAS-R04.root 9 "XMin:XMax:Y:StatLow:StatHigh:SysLow:SysHigh:GlobalLow:GlobalHigh"

#./Execute
./ExecuteGraph
ln -sf Graph_CMS-R02.root Graph_pp_CMSR2.root
ln -sf Graph_CMS-R03.root Graph_pp_CMSR3.root
ln -sf Graph_CMS-R04.root Graph_pp_CMSR4.root
ln -sf Graph_CMS-R06.root Graph_pp_CMSR6.root
ln -sf Graph_CMS-R08.root Graph_pp_CMSR7.root
ln -sf Graph_CMS-R10.root Graph_pp_CMSR8.root
ln -sf Graph_ATLAS-R04.root Graph_pp_ATLASR4.root



