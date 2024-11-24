<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $temperature = $_POST['temperature'];
    $humidity = $_POST['humidity'];
    $date = $_POST['date'];
    $sql_query="INSERT INTO dht (temperature, humidity, date) VALUES ('$temperature','$humidity','$date')";
    $connect= mysqli_connect("mysql", "user", "password", "sensors");
    mysqli_query($connect,$sql_query);
}
?>