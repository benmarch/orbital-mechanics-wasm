import Component, { html } from '../Component.js';

class MainMenu extends Component {
    static get template() {
        return html`

        `;
    }
    constructor() {
        super();


    }
}

window.customElements.define('main-menu', MainMenu);