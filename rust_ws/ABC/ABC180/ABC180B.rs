use proconio::input;

fn main() {
    proconio::input! {
        x: [f64],
    }

    let mut manhattan = 0.0_f64;
    let mut euclidean = 0.0_f64;
    let mut chebyshev = 0.0_f64;
    for &p in &x {
        manhattan += p.abs();
        euclidean += p.abs() * p.abs();
        chebyshev = chebyshev.max(p.abs());
    }
    euclidean = euclidean.sqrt();
    println!("{}", manhattan);
    println!("{}", euclidean);
    println!("{}", chebyshev);
}
