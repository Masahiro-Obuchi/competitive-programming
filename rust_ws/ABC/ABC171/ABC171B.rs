use proconio::input;

fn main() {
    input! {
        n: usize,
        k: usize,
        mut v:[i32; n],
    }
    v.sort();
    let mut ans = 0;
    for i in 0..k {
        ans += v[i];
    }
    println!("{}", ans);
}
