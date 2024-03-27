document.addEventListener("DOMContentLoaded", function() {
    var overlays = document.querySelectorAll('.overlay');
    overlays.forEach(function(overlay) {
        overlay.addEventListener('mouseenter', function() {
            overlay.querySelector('p').style.display = 'block';
            overlay.querySelector('a').style.display = 'block';
        });
        overlay.addEventListener('mouseleave', function() {
            overlay.querySelector('p').style.display = 'none';
            overlay.querySelector('a').style.display = 'none';
        });
    });
});