export default const parseResponse(response) {
	const res = response.split('|');
	return { status: res[0], data: res[1] };
}

export default const parseCommand(command, argument = null) {
	const data = argument || null;
	return `${command}|${data}`;
}