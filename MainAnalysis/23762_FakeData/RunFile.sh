

Prefix=$1
Suffix=$2
YieldPrefix=$3
YieldSuffix=$4
PriorChoice=$5
Fraction=$6
IsPP=$7

#JetR=`DHQuery GlobalSetting.dh Global JetR | sed 's/"//g'`
#JetR="1 2 3 4 6 7 8 9"; \
JetR="4"; \

Centrality=`DHQuery GlobalSetting.dh Global Centrality | sed 's/"//g'`
if [[ "$IsPP" == "1" ]]; then
   Centrality="Inclusive"
fi

for R in $JetR
do
   for C in $Centrality
   do
      Underflow=`DHQuery GlobalSetting.dh Binning PTUnderflow_R${R}_Centrality${C}`
      Overflow=`DHQuery GlobalSetting.dh Binning PTOverflow_R${R}_Centrality${C}`

      Prior=
      if [[ $PriorChoice == "None" ]]; then
         Prior=
      else
         Prior="_${PriorChoice}Prior"
         #Prior=${PriorChoice}Prior
      fi
   
      ./Execute \
         --MC Input/${Prefix}_R${R}_Centrality${C}_${Suffix}.root \
         --Shape PerfectInput/${Prefix}_R${R}_Centrality${C}_${Suffix}${Prior}_PerfectReco.root \
         --Yield Input/${YieldPrefix}_R${R}_Centrality${C}_${YieldSuffix}.root \
         --Output Output_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}${Prior}_Toy.root \
         --Underflow $Underflow --Overflow $Overflow \
         --Fraction $Fraction

      echo  ./Execute \
         --MC Input/${Prefix}_R${R}_Centrality${C}_${Suffix}.root \
         --Shape PerfectInput/${Prefix}_R${R}_Centrality${C}_${Suffix}${Prior}_PerfectReco.root \
         --Yield Input/${YieldPrefix}_R${R}_Centrality${C}_${YieldSuffix}.root \
         --Output Output_R4/${Prefix}_R${R}_Centrality${C}_${Suffix}${Prior}_Toy.root \
         --Underflow $Underflow --Overflow $Overflow \
         --Fraction $Fraction
 
   done
done


