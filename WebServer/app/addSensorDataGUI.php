<?php
    date_default_timezone_set("Europe/Bucharest");
    function valid_user_data() { 
        if ($_SERVER["REQUEST_METHOD"] == "POST") {
            $temperature = $_POST['temperature'];
            $humidity = $_POST['humidity'];
            $date = date("Y-m-d H:i:s");
            $sql_query="INSERT INTO dht (temperature, humidity, date) VALUES ('$temperature','$humidity','$date')";
            $connect= mysqli_connect("mysql", "user", "password", "sensors");
            mysqli_query($connect,$sql_query);
        }
        else {
            echo 'GET request';
        }
        return TRUE; 
    } 
    
    if(valid_user_data()) { 
        header('Location: index.php'); 
    } 
?>