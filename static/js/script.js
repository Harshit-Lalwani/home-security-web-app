// let Data = {{ data | tojson | safe }};

setInterval(function() {
    $.getJSON('/fetch', function(data) {
        $('#gate').prop('checked', data.Gate);
        $('#door').prop('checked', data.Door);
        $('#window').prop('checked', data.Window);
    });
}, 500);

function updateData(gate, door, window) {
    // let Data = {{ data | tojson | safe }};
    fetch('/update', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            Gate: gate,
            Door: door,
            Window: window
        })
    })
    .then(response => response.json())
    .then(data => console.log(data))
    .catch((error) => {
        console.error('Error:', error);
    });
}
