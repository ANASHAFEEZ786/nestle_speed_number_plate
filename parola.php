<?php
// Connect to the database
$servername = "localhost";
$username = "root";
$password = "";
$database = "automation";

$conn = new mysqli($servername, $username, $password, $database);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Check if the API endpoint is called with a string parameter
if ($_SERVER['REQUEST_METHOD'] === 'GET' && isset($_GET['string'])) {
    // Store the string in the database
    $string = $_GET['string'];
    
    // Check if the string already exists in the database
    $sql = "SELECT * FROM parola";
    $result = $conn->query($sql);
    
    if ($result->num_rows > 0) {
        // Update the existing string
        $row = $result->fetch_assoc();
        $sql = "UPDATE parola SET value='$string' WHERE id=" . $row['id'];
    } else {
        // Insert the new string
        $sql = "INSERT INTO parola (value) VALUES ('$string')";
    }
    
    if ($conn->query($sql) === TRUE) {
        echo "String stored successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
} elseif ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Fetch the stored string
    $sql = "SELECT * FROM parola";
    $result = $conn->query($sql);
    
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        echo  $row['value'];
    } else {
        echo "No string stored yet";
    }
} else {
    echo "Unsupported request method";
}

$conn->close();
?>
