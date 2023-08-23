use proconio::input;

fn main() {
    proconio::input! {
        n: i32,
        m: i32,
    }

    if n == m {
        println!("Yes");
    } else {
        println!("No");
    }
}
