#!/usr/bin/env php
<?php
$x = file_get_contents("output.txt");
$y = file_get_contents("validation.txt");
$c = 0;
for ($i = 0; $i < min(strlen($x), strlen($y)); $i++)
  if ($x[$i] != $y[$i])
    $c++;
echo "Diff count: $c\n";
