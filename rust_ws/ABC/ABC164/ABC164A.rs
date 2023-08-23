use proconio::input;

fn main() {
    proconio::input! {
        s: i32,
        w: i32,
    }

    if w >= s {
        println!("unsafe");
    } else {
        println!("safe");
    }
}
