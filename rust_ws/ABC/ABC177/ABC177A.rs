use proconio::input;

fn main() {
    proconio::input! {
        d: i32,
        t: i32,
        s: i32,
    }

    if (d + s - 1) / s <= t {
        println!("Yes");
    } else {
        println!("No");
    }
}
