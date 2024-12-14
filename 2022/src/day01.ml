
let take lst n =
    let rec _take n acc = function
        | [] -> List.rev acc
        | _ when n = 0 -> List.rev acc
        | x :: xs -> _take (n - 1) (x :: acc) xs
    in
    _take n [] lst

let part1 lines = 
    let rec part1' m c lines =
        match lines with
        | [] -> m
        | "" :: xs -> part1' (max m c) 0 xs
        | x :: xs -> part1' m (c + int_of_string x) xs
    in part1' 0 0 lines

let part2 lines = 
    let rec part2' m c lines =
        match lines with
        | [] -> m
        | "" :: xs -> part2' (c :: m) 0 xs
        | x :: xs -> part2' m (c + int_of_string x) xs
    in
    let snacks = List.sort (fun x y -> -(Int.compare x y)) @@ part2' [] 0 lines in
    List.fold_left (+) 0 ( take snacks 3 )

let solve filename = 
    let contents = In_channel.with_open_bin filename In_channel.input_all in
    let lines = String.split_on_char '\n' contents in
    let res1 = part1 lines in
    let res2 = part2 lines in
    print_endline (string_of_int res1);
    print_endline (string_of_int res2)


let () =
    solve "data/day01e.txt"
