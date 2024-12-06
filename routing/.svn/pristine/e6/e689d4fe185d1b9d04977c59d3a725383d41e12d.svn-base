#!/bin/bash

# Array of test case file names
test_cases=(
  "input1.txt"
  "input2.txt"
  "input3.txt"
)

# Iterate over each test case
for test_case in "${test_cases[@]}"
do
  # Extract the test case number from the file name
  test_number=$(echo "$test_case" | grep -oE '[0-9]+')

  echo "Running test case: $test_case"
  
  # Run your Dijkstra's algorithm implementation with the test case input
 python dijkstra < "$test_case" > "output$test_number.txt"
  
  # Compare the output with the expected output
  if diff -q "output$test_number.txt" "expected_output$test_number.txt" > /dev/null; then
    echo "Test case $test_number passed!"
  else
    echo "Test case $test_number failed!"
    # echo "Expected output:"
    # cat "expected_output$test_number.txt"
    # echo "Actual output:"
    # cat "output$test_number.txt"
  fi
  
  echo "------------------------"
done