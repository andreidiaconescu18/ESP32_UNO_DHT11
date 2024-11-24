<?php
    if ($_SERVER["REQUEST_METHOD"] == "GET") {
        $sql_query="SELECT * from dht";
        $connect= mysqli_connect("mysql", "user", "password", "sensors");
        $search_result = mysqli_query($connect,$sql_query);
        
    }
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Sensor page</title> 
    </head>
    <body>
        <h1>Sensor page</h1>
        <form action="addSensorDataGUI.php" method="POST">
            Temperature: <input type="text" name="temperature"><br>
            Humidity: <input type="text" name="humidity"><br>
            <br>
            <input type="submit" name="insert">
        </form>
        <br>
        <br>
        <table border="1" align="left">
            <tr align="center">
                <td>ID</td>
                <td>TEMPERATURE</td>
                <td>HUMIDITY</td>
                <td>DATE</td>
            </tr>
                
            <?php
            while($row=mysqli_fetch_array($search_result)):?>
            <tr align="center">
                <td><?php echo $row['id'];?></td>
                <td><?php echo $row['temperature'];?>°C</td>
                <td><?php echo $row['humidity'];?>%</td>
                <td><?php echo $row['date'];?></td>
            </tr>
            <?php endwhile;?>
		</table>
    </body>
</html>