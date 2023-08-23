use proconio::input;

fn main() {
    proconio::input! {
        k: i32,
        a: i32,
        b: i32,
    }

    if b / k - (a - 1) / k > 0 {
        println!("OK");
    } else {
        println!("NG");
    }
}
