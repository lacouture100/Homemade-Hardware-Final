//Load when the DOM is loaded
window.addEventListener('DOMContentLoaded', () => {
	console.log("DOM Loaded");
	loadTap();
})

var socket = new WebSocket("ws://localhost:8081");

// Listen for messages
socket.addEventListener('message', function (event) {
	console.log('Message from server ', event.data);
});

function loadTap() {

	var item1 = document.querySelector(".item1");	
	item1.onclick = async () => {
		console.log("clicked!");
		// Connection opened
			socket.send('fake tap!');
		
	}
	var item2 = document.querySelector(".item2");
	console.log("found it!");
	
	item2.onclick = async () => {
		console.log("clicked!");
		// Connection opened
			socket.send('fake tap 2!');
		
	}

}