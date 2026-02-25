function get_root() {
    // The plasmoid.file method seems to be missing, so this workaround was used
    var path = plasmoid.metaData.fileName
    path = path.split('/')
    path[path.length - 1] = 'contents'
    return path.join('/')
}

function get_scripts_root() {
    return get_root() + '/scripts'
}

function chdir_scripts_root() {
    return 'cd "'+get_scripts_root()+'";'
}

function create_venv() {
    return (
        chdir_scripts_root() +
        'test -d venv || python3 -m venv venv;' +
        '. venv/bin/activate;' +
        'until curl -f -LI https://pypi.org/simple; do sleep 1; done;' +
        'pip install -r requirements.txt;' +
        'touch venv/.created;' +
        'echo "venv_created"'
    )
}

function activate_venv() {
    return (
        chdir_scripts_root() +
        'until test -f venv/.created; do sleep 1; done;' +
        '. venv/bin/activate;'
    )
}

function random(seed) {
    var x = Math.sin(seed*1000) * 10000;
    return x - Math.floor(x);
}
