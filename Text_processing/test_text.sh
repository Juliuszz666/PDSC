runs=499
i=0
while [ $i -le $runs ]
do
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./text <lorem.txt >> valgrind.txt 2>&1
    i=$((i+1))
	clear
	echo $i
	
done
phrase_1="0 errors from 0 contexts"
phrase_2="no leaks are possible"
file="valgrind.txt"
if [[ -f "$file" ]]; then
	clear
	count_errs=$(grep -o -i "$phrase_1" "$file" | wc -l)
	count_leak=$(grep -o -i "$phrase_2" "$file" | wc -l)
	echo "$count_errs of $((runs+1)) without errors"
	echo "$count_leak of $((runs+1)) without leaks"
else
	echo "File does not exist"
fi


