// Channel to communicate with the browser
const Browser = (window as any).browser;

// Find the currently active tab
function get_current_tab() {
    return new Promise((resolve, reject) => {
        Browser.tabs.query({active: true, lastFocusedWindow: true}, (tab) => {
            if (tab === undefined) {
                reject("Error retrieving tab");
            }
            resolve(tab[0]);
        });
    });
}

// Run a javascript file on a tab. When it's completed 
function run_on_tab(tab, file) {
    return new Promise((resolve, reject) => {
        Browser.tabs.executeScript(tab.id, {
            file,
        }, resolve );
    });
}

function activate() {
    get_current_tab().then(tab => {
        return run_on_tab(tab, "/popup/injected.js");
    })
        .then(() => {
            console.log("Execution successful");  // Back on the extension
        });
}

document.getElementById("run_button").onclick = activate;
