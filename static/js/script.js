document.addEventListener('DOMContentLoaded', function() {
    // Update switches when the database is updated
    setInterval(function() {
        fetch('/data')
        .then(response => response.json())
        .then(data => {
            // Update the switches based on the data
            document.getElementById('gate').checked = data.Gate;
            document.getElementById('door').checked = data.Door;
            document.getElementById('window1').checked = data.Window1;
        });
    }, 1000);  // Update every second

    // Update the database when the switches are triggered
    document.querySelectorAll('.switch').forEach(function(switchElement) {
        switchElement.addEventListener('change', function() {
            var data = {
                'Username': 'user',
                'Time_stamp': Date.now(),
                'Gate': document.getElementById('gate').checked ? 1 : 0,
                'Door': document.getElementById('door').checked ? 1 : 0,
                'Window1': document.getElementById('window1').checked ? 1 : 0,
                'Alerts': 'No Alerts'
            };
            fetch('/update', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(data),
            })
            .then(response => response.json())
            .then(data => {
                console.log('Database updated');
            });
        });
    });
});