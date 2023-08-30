use proconio::input;

fn main() {
    proconio::input! {
        v: [i64],
    }

    let sum: i64 = v
        .iter()
        .scan(0, |max, &x| {
            *max = x.max(*max);
            Some(*max - x)
        })
        .sum();
    println!("{}", sum);
}
