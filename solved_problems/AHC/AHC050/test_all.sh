#! /bin/bash
pushd tools/

exefile=../AHC050

in_files="./in/*"

score_sum=0

num=0

for in_file in $in_files; do
    # 標準エラー出力でスコアを算出して、それを足し合わせる
    out_file=./out/out$(printf "%04d" $num).txt
    touch $out_file
    echo input file is $in_file , out_file is $out_file
    $exefile < $in_file > $out_file
    # cargo run --release --bin tester $exefile < $in_file > $out_file
    score=$((cargo run --release --bin vis $in_file $out_file) |& grep "Score" |& grep -o "[0-9]*")
    mv vis.html vis/vis$(printf "%04d" $num).html
    echo test $num score is $score
    score_sum=$(echo "$score_sum+$score" | bc)
    let num++
done

popd

echo The result of test: Score sum is $score_sum.
echo The test is finished...
