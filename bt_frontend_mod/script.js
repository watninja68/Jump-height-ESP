var socket = io('http://127.0.0.1:5000');

// Keep track of the previous dataset
var previousData = [];

// Function to fetch data from the server
function fetchData() {
    console.log("Sending 'get_data' event to the server...");
    socket.emit('get_data');
}

// Event handler for successful WebSocket connection
socket.on('connect', function() {
    console.log("WebSocket connected.");
    // Fetch data immediately after successful connection
    fetchData();
    // Set up polling to fetch data every 5 seconds
    setInterval(fetchData, 5000); // Fetch data every 5 seconds
});

// Event handler for receiving data from the server
socket.on('data_response', function(data) {
    console.log("Received data from server:", data); // Log received data
    updateCharts(data);
    updateStatistics(data);
    notifyNewData(data); // Call function to display notification for new data

    // Update previousData with the new dataset
    previousData = data.slice();
});
// Function to update charts
function updateCharts(data) {
    // Extract horizontal and vertical data from received data
    var horizontalData = data.map(item => item[0]);
    var verticalData = data.map(item => item[1]);

    // Update myChart (bar chart)
    myChart.data.labels = horizontalData;
    myChart.data.datasets[0].data = verticalData;

    // Update chart (line chart)
    chart.data.labels = horizontalData.slice(0, 5); // Use first 5 values for line chart
    chart.data.datasets[0].data = verticalData.slice(0, 5); // Use first 5 values for line chart

    // Update charts
    myChart.update();
    chart.update();
}

// Function to update statistics
function updateStatistics(data) {
    // Initialize counts
    var horizontalCount = 0;
    var verticalCount = 0;

    // Count jumps
    data.forEach(function(item) {
        if (item[1] > item[0]) {
            verticalCount++;
        } else {
            horizontalCount++;
        }
    });

    // Update jump count statistics in the UI
    document.getElementById("horizontalJumpValue").textContent = horizontalCount; // Update horizontal jumps count
    document.getElementById("verticalJumpValue").textContent = verticalCount; // Update vertical jumps count
    var maxVerticalJump = Math.max(...data.map(item => item[1]));

    // Update best jump value
    document.getElementById("bestjumpvalue").textContent = maxVerticalJump;
}

function notifyNewData(data) {
    // Check if new entries have been added
    var newEntries = data.filter(entry => !previousData.includes(entry));
    if (newEntries.length > 0) {
        // Get the latest entry from the new entries
        var latestEntry = newEntries[0];
        var horizontalJump = latestEntry[0];
        var verticalJump = latestEntry[1];

        // Perform analysis
        var analysis = getJumpAnalysis(horizontalJump, verticalJump);

        // Display notification message with analysis on the website
        var notificationElement = document.getElementById('notification');
        notificationElement.innerHTML = `
            <p><h4><strong>Your Latest Jump</strong></h4></p>
            <p><strong>Horizontal Jump:</strong> ${horizontalJump} meters</p>
            <p><strong>Vertical Jump:</strong> ${verticalJump} meters</p>
            ${analysis}
        `;
    }
}

// Function to perform jump analysis
function getJumpAnalysis(horizontalJump, verticalJump) {
    // Perform your jump analysis here
    var jumpType = horizontalJump < verticalJump ? "Vertical" : "Horizontal";
    var angle = Math.abs(Math.atan(verticalJump / horizontalJump) * (180 / Math.PI));
    var roundedAngle = angle.toFixed(1); // Round angle to 1 decimal place
    var advice = "";
    
    if (jumpType === "Vertical" && angle !== 90) {
        advice = `Try to jump straighter by approximately ${Math.abs(90 - roundedAngle)} degrees to the center.`;
    } else if (jumpType === "Horizontal" && angle !== 45) {
        var difference = Math.abs(45 - roundedAngle);
        if (roundedAngle < 45) {
            advice = `Try to jump at an angle of approximately 45 degrees, ${difference} degrees higher.`;
        } else {
            advice = `Try to jump at an angle of approximately 45 degrees, ${difference} degrees lower.`;
        }
    }
    
    return `
        <p><strong>Type:</strong> ${jumpType}</p>
        <p><strong>Angle:</strong> ${roundedAngle} degrees</p>
        <p><strong>Advice: </strong>${advice}</p>
    `;
}

// Chart initialization (your existing chart initialization code remains the same)
// ...



function updateCharts(data) {
  // Extract horizontal and vertical data from received data
  var horizontalData = data.map(item => item[0]);
  var verticalData = data.map(item => item[1]);

  // Update myChart (bar chart)
  myChart.data.labels = horizontalData;
  myChart.data.datasets[0].data = verticalData;

  // Update chart (line chart)
  chart.data.labels = horizontalData.slice(0, 5); // Use first 5 values for line chart
  chart.data.datasets[0].data = verticalData.slice(0, 5); // Use first 5 values for line chart

  // Update charts
  myChart.update();
  chart.update();
}


// Chart initialization
var myChart = new Chart(document.getElementById('myChart'), {
    type: 'bar',
    data: {
        labels: [],
        datasets: [{
            label: 'Vertical Jump',
            data: [],
            backgroundColor: '#0d6efd',
            borderColor: 'transparent',
            borderWidth: 2.5,
            barPercentage: 0.4,
        }]
    },
    options: {
        scales: {
            yAxes: [{
                scaleLabel: {
                    display: true,
                    labelString: 'Vertical Distance'
                },
                gridLines: {},
                ticks: {
                    stepSize: 15,
                },
            }],
            xAxes: [{
                scaleLabel: {
                    display: true,
                    labelString: 'Horizontal Distance'
                },
                gridLines: {
                    display: false,
                }
            }]
        }
    }
});

var chart = new Chart(document.getElementById('myChart2'), {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Vertical Jump',
            data: [],
            backgroundColor: 'transparent',
            borderColor: '#d9534f', // Change color to red
            lineTension: 0.4,
            borderWidth: 1.5,
        }]
    },
    options: {
        scales: {
            yAxes: [{
                scaleLabel: {
                    display: true,
                    labelString: 'Vertical Distance'
                },
                gridLines: {
                    drawBorder: false
                },
                ticks: {
                    stepSize: 12,
                }
            }],
            xAxes: [{
                scaleLabel: {
                    display: true,
                    labelString: 'Horizontal Distance'
                },
                gridLines: {
                    display: false,
                }
            }]
        }
    }
});

var chart1 = document.getElementById('chart3');
var myChart1 = new Chart(chart1, {
  type: 'line',
  data: {
    labels: ["One", "Two", "Three", "Four", "Five", 'Six', "Seven", "Eight"],
    datasets: [{
      label: "User1",
      lineTension: 0.2,
      borderColor: '#d9534f',
      borderWidth: 1.5,
      showLine: true,
      data: [3, 30, 16, 30, 16, 36, 21, 40, 20, 30],
      backgroundColor: 'transparent'
    }, {
      label: "User2",
      lineTension: 0.2,
      borderColor: '#5cb85c',
      borderWidth: 1.5,
      data: [6, 20, 5, 20, 5, 25, 9, 18, 20, 15],
      backgroundColor: 'transparent'
    },
               {
                 label: "User3",
                 lineTension: 0.2,
                 borderColor: '#f0ad4e',
                 borderWidth: 1.5,
                 data: [12, 20, 15, 20, 5, 35, 10, 15, 35, 25],
                 backgroundColor: 'transparent'
               },
               {
                 label: "User4",
                 lineTension: 0.2,
                 borderColor: '#337ab7',
                 borderWidth: 1.5,
                 data: [16, 25, 10, 25, 10, 30, 14, 23, 14, 29],
                 backgroundColor: 'transparent'
               }]
  },
  options: {
    scales: {
      yAxes: [{
        gridLines: {
          drawBorder: false
        },
        ticks: {
          stepSize: 12
        }
      }],
      xAxes: [{
        gridLines: {
          display: false,
        },
      }],
    }
  }
}

);
