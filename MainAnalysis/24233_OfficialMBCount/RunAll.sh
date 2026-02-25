#!/bin/bash

#JetR=`DHQuery GlobalSetting.dh Global JetR | tr -d '"'`
JetR="1 2 3 4 6 7 8 9"; \
Centrality=`DHQuery GlobalSetting.dh Global Centrality | tr -d '"'`

#OfficialCount=11775759052
#OfficialLumi=1695.62
#OfficialCount=13477839140.0
OfficialCount=13744794140.0
OfficialLumi=1668.26 #mubarn ^-1
PbPbCrosssection=8.239 #barn
#PbPbCrosssection=7.67 #barn//used from 2017 assumption
#OfficialCount=12795554200.0

for R in $JetR
do
   for C in $Centrality
   do
      # echo $R $C

      Lumi=`DHQuery GlobalSetting.dh Lumi PbPbData_R${R}_Centrality${C}_BRIL | tr -d '"'`
      CMin=`DHQuery GlobalSetting.dh CentralityMin $C`
      CMax=`DHQuery GlobalSetting.dh CentralityMax $C`
#      CRange=`echo $CMax | SubtractConst $CMin`
      CRange=$(echo "$CMax - $CMin" | bc -l)
       echo "Lumi: $Lumi $OfficialLumi"
       echo "CRange: $CMin $CMax $CRange"

 #     NMB=`echo $OfficialCount | DivideConst $OfficialLumi | MultiplyConst $Lumi`
#      NMB=`echo $NMB | DivideConst 0.9 | MultiplyConst $CRange`

      if [[ -z "$OfficialLumi" || "$OfficialLumi" == "0" ]]; then
	  echo "Error: OfficialLumi is zero/empty" >&2
	  NMB=0
      else
	  #NMB=$(echo "scale=10; ($OfficialCount / $OfficialLumi) * $Lumi * $CRange" | bc -l)
	  NMB=$(echo "scale=20; ($OfficialCount / $OfficialLumi) * $Lumi * $CRange" | bc -l | xargs printf "%.0f")
      fi
      # First compute OfficialCount / OfficialLumi * Lumi
      #NMB=$(echo "scale=10; ($OfficialCount / $OfficialLumi) * $Lumi * $CRange" | bc -l)

      # Then divide by 0.9 and multiply by CRange
      #NMB=$(echo "scale=10; ($NMB / 0.9) * $CRange" | bc -l)

      DHSet GlobalSetting.dh MBCount PbPbData_R${R}_Centrality${C}_OfficialCount string $NMB
   done
done
