use proconio::input;

fn main() {
    input! {
        mut l:[u64],
    }
    l.sort();
    let mut count = 0;
    for k in 0..l.len() {
        for j in 0..k {
            for i in 0..j {
                // ソートしてあるので、l[i] <= l[j] <= l[k] が成り立つ
                if l[i] != l[j] && l[j] != l[k] && l[k] < l[i] + l[j] {
                    count += 1;
                }
            }
        }
    }
    println!("{}", count);
}
